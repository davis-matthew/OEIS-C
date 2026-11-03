/*
 * A230241  Number of ways to write n = p + q with p, 3*p - 10 and (p-1)*q - 1
 *          all prime, where q is a positive integer.
 * https://oeis.org/A230241
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ways to write n = p + q with p, 3p‑10, and
 *            (p-1)q‑1 all prime. All computations are performed with idx_t
 *            for indices and seq_t for the result
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 116 terms through n = 9638 (2026-11-03)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Auxiliary support from the utils package            */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* Upper bound for the sieve – sufficient for n up to ~10 000. */
#define SIEVE_MAX 25000000UL

/* Prime sieve and list – lazily initialized. */
static unsigned char *prime_is = NULL;  /* 0/1 flags, size SIEVE_MAX+1 */
static idx_t *prime_list = NULL;        /* dynamic array of primes */
static size_t prime_cnt = 0;            /* number of primes stored */

/* One‑time initialization of the prime data structures. */
static void init_prime_data(void)
{
    if (prime_is != NULL) return; /* already initialized */

    prime_is = (unsigned char *)malloc(SIEVE_MAX + 1);
    if (prime_is == NULL) return; /* again, fall back to test only */

    /* Mark all numbers as prime initially. */
    for (size_t i = 0; i <= SIEVE_MAX; ++i) prime_is[i] = 1;
    prime_is[0] = prime_is[1] = 0;

    /* Sieve of Eratosthenes. */
    for (idx_t i = 2; i * i <= SIEVE_MAX; ++i) {
        if (prime_is[i]) {
            for (idx_t j = i * i; j <= SIEVE_MAX; j += i)
                prime_is[j] = 0;
        }
    }

    /* Count primes to size the list. */
    for (idx_t i = 2; i <= SIEVE_MAX; ++i)
        if (prime_is[i]) ++prime_cnt;

    prime_list = (idx_t *)malloc(prime_cnt * sizeof(idx_t));
    if (prime_list == NULL) return; /* allocation failure */

    size_t pos = 0;
    for (idx_t i = 2; i <= SIEVE_MAX; ++i)
        if (prime_is[i]) prime_list[pos++] = i;
}

/* Prüfen, ob ein Wert <= SIEVE_MAX prim ist. */
static inline bool is_prime_u64(idx_t x)
{
    if (x <= SIEVE_MAX) return prime_is[x] != 0;
    /* For larger values, fall back on the generic test.  The value
       may be beyond the range of seq_t; we cast to seq_t because on a
       build where seq_t is large enough (uint64_t or bigger) this
       works perfectly.  On tiny builds (uint8_t) this branch will not
       be taken because the numbers involved are small enough to fit
       into the sieve. */
    return is_prime_big((seq_t)x);
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* offset is 1, but guard anyway */

    init_prime_data();

    seq_t count = 0;

    /* Iterate over all primes p < n. */
    for (size_t idx = 0; idx < prime_cnt; ++idx) {
        idx_t p = prime_list[idx];
        if (p >= n) break;

        /* Check primality of 3p‑10. */
        idx_t val3 = 3 * p - 10;
        if (!is_prime_u64(val3)) continue;

        /* Compute (p‑1)q‑1 with q = n‑p. */
        idx_t prod = (p - 1) * (n - p);
        prod -= 1; /* (p‑1)q‑1 */
        if (prod < 2) continue; /* must be at least 2 to be prime */

        if (!is_prime_u64(prod)) continue;

        ++count;
    }

    return count;
}
