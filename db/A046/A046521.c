/*
 * A046521  Array T(i,j) = binomial(-1/2-i,j)*(-4)^j, i,j >= 0 read by
 *          antidiagonals going down.
 * https://oeis.org/A046521
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) is the antidiagonal reading of the array T(i,j) =
 *            binomial(-½ - i , j) * (-4)^j (i,j ≥ 0). For a given n the
 *            indices are s = floor((√(8n+1) - 1)/2); // diagonal number i = n
 *            - s*(s+1)/2; // offset along the diagonal j = s - i; //
 *            remaining offset The value T(i,j) is computed by the
 *            recurrenc...
 * Range:     u8 n <= 15, u16 n <= 45, u32 n <= 136, u64 n <= 467,
 *            u128 n <= 1325, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find the diagonal number s with s*(s+1)/2 ≤ n < (s+1)*(s+2)/2 */
    idx_t s = 0;
    while (1) {
        idx_t next = ((s + 1) * (s + 2)) / 2;
        if (next <= n)
            ++s;
        else
            break;
    }

    /* i = offset along the diagonal, j = remaining index */
    idx_t i = n - (s * (s + 1)) / 2;
    idx_t j = s - i;

    /* Compute T(i,j) by the recurrence */
    if (j == 0)
        return (seq_t)1;

    seq_t res = 1;
    for (idx_t k = 1; k <= j; ++k) {
        idx_t term = 2 * i + 2 * k - 1;          /* 2*i + 2*k - 1  */
        __uint128_t prod = (__uint128_t)res * (2 * term);
        prod /= k;
        res = (seq_t)prod;
    }
    return res;
}
