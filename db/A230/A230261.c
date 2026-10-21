/*
 * A230261  Number of ways to write 2*n - 1 = p + q with p, p + 6 and q^4 + 1
 *          all prime, where q is a positive integer.
 * https://oeis.org/A230261
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A230261:
 *  a(n) = number of ways to write 2*n-1 = p + q with
 *  p prime, p+6 prime, and q^4+1 prime, where q>0.
 *  The implementation pre‑computes a sieve of primes up to
 *  LIMIT and a flag array for q^4+1 primality.
 *  It then scans all admissible q for a given n.
 *  The algorithm is O(n) per call and works for all n up to ~10 000
 *  (LIMIT = 21000 comfortably covers 2*n+6 for those n).
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/*--- deterministic Miller-Rabin for 64‑bit integers ---------------------------------*/
static bool is_prime_u64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small_primes[] = {
        2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL
    };
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        if (n == small_primes[i]) return true;
        if (n % small_primes[i] == 0) return false;
    }
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1ULL) == 0ULL) { d >>= 1; ++s; }

    static const uint64_t bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL,
        450775ULL, 9780504ULL, 1795265022ULL
    };
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0ULL) continue;
        __uint128_t x = 1, p = a, e = d;
        while (e) {
            if (e & 1ULL) x = (x * p) % n;
            p = (p * p) % n;
            e >>= 1;
        }
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (int r = 1; r < s; ++r) {
            x = (x * x) % n;
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/*--- precomputation -----------------------------------------------------------*/
#define LIMIT 21000   /* safe upper bound for 2*n+6 when n ≤ 10000 */

static bool initialized = false;
static uint8_t prime_flag[LIMIT + 1];
static uint8_t qflag[LIMIT + 1];

static void init(void)
{
    /* simple sieve of Eratosthenes up to LIMIT */
    for (int i = 0; i <= LIMIT; ++i) prime_flag[i] = 1;
    prime_flag[0] = prime_flag[1] = 0;
    for (int i = 2; i * i <= LIMIT; ++i) {
        if (prime_flag[i]) {
            for (int j = i * i; j <= LIMIT; j += i)
                prime_flag[j] = 0;
        }
    }

    /* flag for q^4 + 1 being prime, for q = 1 .. LIMIT */
    for (int q = 1; q <= LIMIT; ++q) {
        __uint128_t q128 = (uint64_t)q;
        __uint128_t q4 = q128 * q128 * q128 * q128;  /* q^4 fits in 128 bits */
        uint64_t val = (uint64_t)q4 + 1ULL;
        qflag[q] = is_prime_u64(val) ? 1 : 0;
    }

    initialized = true;
}

/*--- main sequence function ----------------------------------------------------*/
seq_t a(idx_t n)
{
    if (!initialized) init();

    if (n < 1) return 0;            /* below the OEIS offset */

    /* target = 2*n - 1 */
    uint64_t target = (uint64_t)n * 2ULL - 1ULL;
    /* for n = 1, target = 1; no solutions */
    if (target < 3) return 0;

    uint64_t qmax = target >= 2 ? target - 2 : 0;   /* q ≤ target-2 */
    seq_t count = 0;

    /* iterate over admissible q; restrict to LIMIT for flags */
    uint64_t limit_q = qmax < LIMIT ? qmax : LIMIT;
    for (uint64_t q = 1; q <= limit_q; ++q) {
        if (!qflag[q]) continue;               /* q^4+1 not prime */

        uint64_t p = target - q;               /* p = target - q */
        if (p < 2) continue;

        /* check p and p+6 */
        if (p + 6ULL > LIMIT) {                /* outside precomputed range */
            if (!is_prime_u64(p)) continue;
            if (!is_prime_u64(p + 6ULL)) continue;
        } else {
            if (!prime_flag[p]) continue;
            if (!prime_flag[p + 6ULL]) continue;
        }
        count++;
    }

    return count;
}
