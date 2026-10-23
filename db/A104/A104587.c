/*
 * A104587  Triangle read by rows, given by the matrix product A * B where A
 *          (A094727) = [1; 2, 3; 3, 4, 5; 4, 5, 6, 7; ...] and B = [1; 1, 1;
 *          1, 1, 1; ...] (both are infinite lower triangular matrices with
 *          the other terms zero).
 * https://oeis.org/A104587
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = ((row - col + 1) * (3*row + col + 2)) / 2 where (row,
 *            col) are the 0‑based row and column indices in the flattened
 *            lower‑triangular array produced by A*B
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t triangular(idx_t r)
{
    return (idx_t)(((unsigned __int128)r * (r + 1)) / 2);
}

static inline idx_t find_row(idx_t n)
{
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        idx_t t = triangular(mid);
        if (t <= n)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}

seq_t a(idx_t n)
{
    idx_t row = find_row(n);
    idx_t col = n - triangular(row);

    unsigned __int128 part1 = (unsigned __int128)(row - col + 1);
    unsigned __int128 part2 = (unsigned __int128)(3 * row + col + 2);
    unsigned __int128 prod = part1 * part2;
    return (seq_t)(prod / 2);
}
