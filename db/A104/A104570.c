/*
 * A104570  Triangle read by rows: T(i,j) is the (i,j)-entry (1 <= j <= i) of
 *          the product R*Q of the infinite lower triangular matrices R = [1;
 *          1,1; 1,1,1; 1,1,1,1; ...] and Q = [1; 1,3; 1,3,1; 1,3,1,3; ...].
 * https://oeis.org/A104570
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(i,j) = (i-j+1) if j is odd, 3*(i-j+1) if j is even. a(n) is
 *            the sequence obtained by reading the lower‑triangular matrix row
 *            by row
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find row i such that S(i-1) < n ≤ S(i), where S(i)=i*(i+1)/2. */
    idx_t low = 1, high = 1;

    /* Increase high until it is large enough. */
    while ((unsigned __int128)high * (high + 1) / 2 < n)
        high <<= 1;

    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if ((unsigned __int128)mid * (mid + 1) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }

    idx_t row = low;                          /* i */
    idx_t prev_sum = (unsigned __int128)(row - 1) * row / 2; /* S(i-1) */
    idx_t col = n - (idx_t)prev_sum;          /* j (1‑based) */

    seq_t val = (seq_t)(row - col + 1);
    if (col % 2 == 0)
        val *= (seq_t)3;

    return val;
}
