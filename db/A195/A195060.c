/*
 * A195060  Numbers on the main diagonals and on the axes (x,y) in the square
 *          spiral whose vertices are the generalized pentagonal numbers
 *          A001318.
 * https://oeis.org/A195060
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0; a(2*n) = A001318(n+1) (n ≥ 1) a(4*n+1) = A032528(n+1)
 *            a(4*n+3) = A045943(n+1) (with n an integer ≥0)
 * Range:     u8 n <= 49, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   A032528, A045943
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/A032528.h"
#include "../utils/A045943.h"
static seq_t A001318(idx_t t)
{
    if (t == 0) return (seq_t)0;
    if (t & 1) { /* t odd */
        idx_t k = (t + 1) / 2;
        return (seq_t)k * ((seq_t)3 * k - 1) / 2;
    } else {     /* t even */
        idx_t k = t / 2;
        return (seq_t)k * ((seq_t)3 * k + 1) / 2;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    if ((n & 1) == 0) {
        idx_t m = n >> 1;        /* m = n/2  (m ≥ 1) */
        return A001318(m + 1);   /* a(2*m) = A001318(m+1) */
    }

    switch (n & 3) {
        case 1:
            /* n = 4*k+1  →  a(n) = A032528(k+1) */
            return A032528((n + 3) >> 2);
        case 3:
            /* n = 4*k+3  →  a(n) = A045943(k+1) */
            return A045943((n + 1) >> 2);
        default:
            /* should never occur */
            return (seq_t)0;
    }
}
