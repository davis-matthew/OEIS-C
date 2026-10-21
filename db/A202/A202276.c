/*
 * A202276  Number of integers k <= n such that sigma(x) = k has no solution,
 *          sigma = A000203.
 * https://oeis.org/A202276
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integers k ≤ n whose sigma value (sum of
 *            divisors) has no preimage, i.e. sigma(x)=k has no integer
 *            solution
 * Range:     u8 n <= 23, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Ensure we have the sum‑of‑divisors values up to n. */
    static seq_t *sigma = NULL;
    static size_t sigma_cap = 0;

    size_t nn = (size_t)n;

    if (nn > sigma_cap) {
        /* free old and allocate new array */
        free(sigma);
        sigma_cap = nn + 1;
        sigma = (seq_t*)calloc(sigma_cap, sizeof(seq_t));
        if (sigma == NULL) return (seq_t)0; /* out of memory – return 0 as sentinel */

        /* compute sigma for 1..nn via simple divisor summing sieve */
        for (size_t i = 1; i <= nn; ++i) {
            for (size_t j = i; j <= nn; j += i) {
                sigma[j] += (seq_t)i;
            }
        }
    }

    /* Mark which k <= n have a preimage under sigma. */
    static bool *present = NULL;
    static size_t present_cap = 0;

    if (nn + 1 > present_cap) {
        free(present);
        present_cap = nn + 1;
        present = (bool*)malloc(present_cap * sizeof(bool));
        if (present == NULL) return (seq_t)0;
    }

    /* Clear previous markings */
    for (size_t i = 1; i <= nn; ++i) present[i] = false;

    for (size_t x = 1; x <= nn; ++x) {
        if (sigma[x] <= nn) {
            present[(size_t)sigma[x]] = true;
        }
    }

    /* Count unattained sigma values */
    seq_t count = (seq_t)0;
    for (size_t k = 1; k <= nn; ++k) {
        if (!present[k]) ++count;
    }

    return count;
}
