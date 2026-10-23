/*
 * A048075  Number of nonempty subsets of {1,2,...,n} in which exactly 3/4 of
 *          the elements are <= (n+2)/3.
 * https://oeis.org/A048075
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1}^{⌊n/4⌋} C(⌊(n+2)/3⌋, 3k) * C(n - ⌊(n+2)/3⌋, k)
 * Range:     u8 n <= 16, u16 n <= 30, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where C(a,b) is the binomial coefficient (a choose b) */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"

seq_t a(idx_t n) {
    idx_t L = (n + 2) / 3;        /* floor((n+2)/3) */
    idx_t maxk = n / 4;           /* maximum possible k */
    seq_t total = 0;

    for (idx_t k = 1; k <= maxk; ++k) {
        /* if 3k > L or k > n-L then subsequent terms vanish */
        if (3 * k > L) break;
        if (k > n - L) break;

        seq_t c1 = binomial(L, 3 * k);
        seq_t c2 = binomial(n - L, k);
        total += c1 * c2;
    }
    return total;
}
