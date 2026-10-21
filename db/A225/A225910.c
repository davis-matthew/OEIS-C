/*
 * A225910  Square array read by antidiagonals: a(m,n) is the number of binary
 *          pattern classes in the (m,n)-rectangular grid, two patterns are in
 *          the same class if one of them can be obtained by a reflection or
 *          180-degree rotation of the other.
 * https://oeis.org/A225910
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = number of binary pattern classes in the (m,n)-rectangular
 *            grid, read by antidiagonals (i.e., a(n) = a(i,j) with (i,j)
 *            computed from n)
 * Range:     u8 n <= 29, u16 n <= 47, u32 n <= 82, u64 n <= 158,
 *            u128 n <= 285, big any n
 * Helpers:   antidiag_idx, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow2.h"
#include "../utils/antidiag_idx.h"

seq_t a(idx_t n)
{
    idx_t i, j;
    antidiag_idx(n, &i, &j);

    /* 0‑dimensional grids have 1 class. */
    if (i == 0 || j == 0)
        return 1;

    /* 1×1 grid: two patterns (0 and 1). */
    if (i == 1 && j == 1)
        return 2;

    bool ieven = (i & 1) == 0;
    bool jeven = (j & 1) == 0;

    idx_t prod = i * j;          /* m*n */
    idx_t half, exp1, exp2;
    seq_t part, inside, result;

    if (ieven && jeven) {
        /* m even, n even: a = 2^(mn/2-2) * (2^(mn/2) + 3) */
        half = prod / 2;          /* mn/2 is integer because both even */
        exp1 = half - 2;          /* mn/2 - 2  */
        part = pow2(exp1);
        inside = pow2(half) + 3;
        result = part * inside;
    }
    else if (ieven && !jeven) {
        /* m even, n odd: a = 2^(mn/2-1) * (2^(mn/2-1) + 2^(m/2-1) + 1) */
        half = prod / 2;          /* floor(mn/2) since mn odd */
        exp1 = half - 1;          /* mn/2 - 1  */
        part = pow2(exp1);
        inside = pow2(exp1) + pow2((i / 2) - 1) + 1;
        result = part * inside;
    }
    else if (!ieven && jeven) {
        /* m odd, n even: symmetric to the previous case */
        half = prod / 2;
        exp1 = half - 1;
        part = pow2(exp1);
        inside = pow2(exp1) + pow2((j / 2) - 1) + 1;
        result = part * inside;
    }
    else {
        /* m odd, n odd: a = 2^((mn-1)/2-1) * (2^((mn-1)/2) + 2^((m-1)/2) + 2^((n-1)/2) + 1) */
        half = (prod - 1) / 2;      /* (mn-1)/2 is integer */
        exp1 = half - 1;            /* (mn-1)/2 - 1 */
        part = pow2(exp1);
        inside = pow2(half)
               + pow2((i - 1) / 2)
               + pow2((j - 1) / 2)
               + 1;
        result = part * inside;
    }

    return result;
}
