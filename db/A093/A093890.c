/*
 * A093890  Number of primes arising as the sum of one or more divisors of n.
 * https://oeis.org/A093890
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct primes that can be expressed as the
 *            sum of one or more positive divisors of n. The algorithm
 *            enumerates all divisors of n, then performs a subset‑sum DP
 *            (bitset) up to σ(n), marks reachable sums, and counts those that
 *            are prime
 * Range:     u8 n <= 539, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_small_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "../utils/is_small_prime.h"

/* ----------------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* offset starts at 1 */

    /* ---------- collect divisors ------------------------------------------ */
    idx_t sq = (idx_t) sqrt((long double) n);
    /* maximum number of divisors for n ≤ 10⁴ is much less than 200 */
    size_t cap = 2 * sq + 2;
    idx_t *divs = (idx_t *) malloc(cap * sizeof(idx_t));
    size_t divc = 0;
    for (idx_t i = 1; i <= sq; ++i) {
        if (n % i == 0) {
            divs[divc++] = i;
            idx_t j = n / i;
            if (j != i) divs[divc++] = j;
        }
    }

    /* ---------- compute sigma (sum of divisors) --------------------------- */
    idx_t sigma = 0;
    for (size_t k = 0; k < divc; ++k) sigma += divs[k];

    /* ---------- subset sum DP --------------------------------------------- */
    /* reachable[s] == 1 if sum s can be obtained from a subset of divisors */
    bool *reachable = (bool *) calloc((size_t)sigma + 1, 1);
    reachable[0] = true;

    for (size_t k = 0; k < divc; ++k) {
        idx_t d = divs[k];
        for (idx_t s = sigma - d; ; --s) {
            if (reachable[s]) reachable[s + d] = true;
            if (s == 0) break;
        }
    }

    /* ---------- count prime sums ------------------------------------------- */
    seq_t count = 0;
    for (idx_t s = 2; s <= sigma; ++s) {
        if (reachable[s] && is_small_prime(s))
            ++count;
    }

    /* ---------- cleanup ----------------------------------------------------- */
    free(divs);
    free(reachable);

    return count;
}
