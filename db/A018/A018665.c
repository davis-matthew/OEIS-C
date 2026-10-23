/*
 * A018665  Divisors of 822.
 * https://oeis.org/A018665
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   Divisors of 822 (with prime factorization 2 * 3 * 137), listed
 *            in ascending order
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static const uint64_t _pf[3] = {2ULL, 3ULL, 137ULL};

seq_t a(idx_t n)
{
    /* There are 2^3 = 8 divisors.  The nth divisor (1‑based) is returned
       for 1 ≤ n ≤ 8; for larger n we return 0 (undefined term). */
    if (n < 1) return 0;
    if (n > 8) return (seq_t)0;

    /* Compute all 8 divisors by enumerating subsets of the 3 prime factors. */
    seq_t divs[8];
    for (idx_t mask = 0; mask < (idx_t)8; ++mask) {
        seq_t prod = (seq_t)1;
        for (idx_t i = 0; i < 3; ++i) {
            if (mask & ((idx_t)1 << i))
                prod = prod * (seq_t)_pf[i];
        }
        divs[mask] = prod;
    }

    /* Sort the 8 values in ascending order (simple bubble sort). */
    for (idx_t i = 0; i < 8; ++i) {
        for (idx_t j = i + 1; j < 8; ++j) {
            if (divs[j] < divs[i]) {
                seq_t tmp = divs[i];
                divs[i] = divs[j];
                divs[j] = tmp;
            }
        }
    }

    return divs[n - 1];
}
