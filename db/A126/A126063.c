/*
 * A126063  Triangle read by rows: see A128196 for definition.
 * https://oeis.org/A126063
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 27, u32 n <= 65, u64 n <= 170,
 *            u128 n <= 434, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A126063:  Triangle read by rows.  
 * The triangle entry T[r][c] (0‑based) is
 *       T[r][c] = 2^c * ((2r-1)!! / (2c-1)!!),
 * where !! denotes the odd double factorial with (-1)!! = 1.
 * The sequence a(n) is the flattening of this triangle by rows.
 */
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t low = 0, high = (1ULL << 32) + 1;   /* 4.3e9 > max possible r for n<2^64 */
    while (low < high) {
        idx_t mid = low + (high - low + 1) / 2;
        __int128 tri = (__int128)mid * (mid + 1) / 2;
        if (tri <= n)
            low = mid;
        else
            high = mid - 1;
    }
    idx_t r = low;

    /* Column c (Offset within the row) */
    idx_t c = n - ((__int128)r * (r + 1) / 2);

    /* Compute product of odd numbers from (2c+1) up to (2r-1) */
    seq_t val = 1;
    for (idx_t k = c + 1; k <= r; ++k) {
        val = val * (seq_t)(2 * k - 1);
    }

    /* Multiply by 2^c */
    val = val * pow2(c);

    return val;
}
