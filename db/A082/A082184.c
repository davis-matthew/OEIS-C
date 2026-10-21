/*
 * A082184  The a(n)-th triangular number is the sum of the n-th triangular
 *          number and the smallest triangular number possible.
 * https://oeis.org/A082184
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the index m of the first triangular number * T_m that
 *            can be written as the sum of the n-th triangular number * T_n
 *            and the smallest triangular number possible
 * Range:     u8 n <= 6, u16 n <= 30, u32 n <= 8955, u64 n <= 8955,
 *            u128 n <= 8955, big any n
 * Helpers:   is_triangular, isqrt_ul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_triangular.h"
#include "../utils/isqrt_ul.h"

/* Local helper: triangular number of index x (unsigned). */
static seq_t triangular(idx_t x)
{
    return (seq_t)x * ((seq_t)x + 1) / 2;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 2) return 0;      /* undefined for n < 2, but safe return */

    seq_t Tn = triangular(n);

    for (idx_t d = 1;; d++) {
        idx_t m = n + d;
        seq_t Tm = triangular(m);
        seq_t diff = Tm - Tn;          /* Tm - Tn = T_k for some k */
        if (is_triangular(diff))
            return (seq_t)m;           /* a(n) = m */
    }
}
