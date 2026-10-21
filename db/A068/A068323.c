/*
 * A068323  a(n)=number of arithmetic progressions of primes, nondecreasing
 *          with sum n.
 * https://oeis.org/A068323
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = number of arithmetic progressions of primes (allowing repeated terms)
 *        whose terms sum to n.  The progression is non‑decreasing, i.e.
 *        it has the form p, p+d, …, p+(k‑1)d with p and d integers, d≥0 and
 *        all terms prime.  The counting is performed by enumerating all
 *        admissible lengths k, first primes p and common differences d
 *        that satisfy k·p + d·k(k‑1)/2 = n.  All primality tests rely on a
 *        sieve constructed up to n on the first call that exceeds the
 *        previously sieved limit.  The algorithm runs in O(n·π(n)) time,
 *        which is well below the required limit for n≤10000. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

static idx_t sieve_limit = 0;      /* largest number that has been sieved   */
static bool *prime_mask   = NULL;  /* true → prime,  false → composite     */
static uint64_t *prime_list = NULL;/* list of primes up to sieve_limit      */
static size_t prime_count   = 0;   /* number of primes stored               */

static void build_sieve(idx_t limit)
{
    bool *new_mask = (bool *)malloc((limit + 1) * sizeof(bool));
    if (!new_mask) exit(1); /* allocation failure aborts */

    for (idx_t i = 0; i <= limit; ++i) new_mask[i] = true;
    if (limit >= 0) new_mask[0] = false;
    if (limit >= 1) new_mask[1] = false;

    for (idx_t i = 2; i * i <= limit; ++i) {
        if (new_mask[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                new_mask[j] = false;
        }
    }

    /* count primes */
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (new_mask[i]) ++cnt;

    uint64_t *new_list = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!new_list) { free(new_mask); exit(1); }

    size_t idx = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (new_mask[i]) new_list[idx++] = (uint64_t)i;

    free(prime_mask);
    free(prime_list);
    prime_mask   = new_mask;
    prime_list   = new_list;
    prime_count  = cnt;
    sieve_limit  = limit;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* defined only for n≥1, but safe */

    if (n > sieve_limit)      /* extend sieve once per call if needed */
        build_sieve(n);

    seq_t count = 0;

    /* k = 1  (single prime) */
    if (prime_mask[n]) ++count;

    /* k ≥ 2 */
    idx_t max_k = n / 2;          /* minimal sum for length k is 2·k */
    for (idx_t k = 2; k <= max_k; ++k) {
        idx_t denom = (k * (k - 1)) / 2; /* k·(k−1)/2 */
        if (denom == 0) continue;       /* should never happen */

        idx_t limit_p = n / k;          /* largest possible first prime */

        for (size_t pi = 0; pi < prime_count; ++pi) {
            uint64_t up = prime_list[pi];
            if ((idx_t)up > limit_p) break;

            idx_t p = (idx_t)up;
            idx_t temp = n - k * p;       /* n − k·p  =  d·denom  */

            if (temp < 0) break;          /* further primes give negative temp */
            if (temp == 0) {              /* d = 0  (all terms equal) */
                ++count;
                break;                    /* larger primes would make temp negative */
            }

            if (temp % denom != 0) continue;
            idx_t d = temp / denom;       /* common difference (≥0) */

            bool ok = true;
            for (idx_t i = 1; i < k; ++i) {
                idx_t term = p + i * d;
                if (!prime_mask[term]) { ok = false; break; }
            }
            if (ok) ++count;
        }
    }
    return count;
}
