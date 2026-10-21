/*
 * A061070  Number of distinct values in the list of values of the Euler
 *          totient function {phi(j) : j=1..n}.
 * https://oeis.org/A061070
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{phi(j) : j=1..n}|, implemented by sieving φ up to n and
 *            counting how many distinct values have appeared
 * Range:     u8 n <= 960, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* seq_t and idx_t are defined by the build.  */
static idx_t current_max = 0;
static seq_t *distinct_arr = NULL;

seq_t a(idx_t n)
{
    /* Offset 1: a(1)=1.  For n==0 return 0 (undefined). */
    if (n == 0) return 0;

    if (n > current_max) {
        /* Allocate and compute φ[1..n] by Euler's sieve. */
        idx_t *phi = (idx_t *)malloc((n + 1) * sizeof(idx_t));
        if (!phi) return 0; /* allocation failure – leave function silent */

        for (idx_t i = 0; i <= n; ++i)
            phi[i] = i;
        for (idx_t i = 2; i <= n; ++i) {
            if (phi[i] == i) {            /* i is prime */
                for (idx_t j = i; j <= n; j += i)
                    phi[j] -= phi[j] / i;
            }
        }
        phi[1] = 1;

        /* Count distinct φ values. */
        bool *seen = (bool *)calloc((n + 1), sizeof(bool));
        seq_t *new_dist = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        if (!seen || !new_dist) {
            free(phi);
            if (seen) free(seen);
            if (new_dist) free(new_dist);
            return 0;
        }

        seq_t count = 0;
        for (idx_t j = 1; j <= n; ++j) {
            idx_t val = phi[j];
            if (!seen[val]) {
                seen[val] = true;
                ++count;
            }
            new_dist[j] = count;
        }

        /* Clean up and replace old data. */
        free(phi);
        free(seen);

        if (distinct_arr) free(distinct_arr);
        distinct_arr = new_dist;
        current_max = n;
    }

    return distinct_arr[n];
}
