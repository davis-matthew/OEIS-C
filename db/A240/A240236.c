/*
 * A240236  Triangle read by rows: sum of digits of n in base k, for 2<=k<=n.
 * https://oeis.org/A240236
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabl, base
 * Formula:   a(n) = sum of digits of the row number of the n‑th * element in
 *            its corresponding base (as described in OEIS A240236). * * The
 *            n‑th term (n ≥ 2) is obtained by: * 1. pos = n – 1 (the 1‑based
 *            position in the flattened triangle) * 2. Find the smallest m ≥ 2
 *            such that m(m − 1)/2 ≥ pos. * 3. k = 1 +...
 * Range:     u8 n <= 4951, u16 n <= 4951, u32 n <= 4951, u64 n <= 4951,
 *            u128 n <= 4951, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Find the row number m for a given position pos (pos ≥ 1).            */
/* It satisfies m(m−1)/2 ≥ pos and (m−1)(m−2)/2 < pos.                     */
static inline idx_t
find_row(idx_t pos)
{
    idx_t low = 2;
    idx_t high = 2;
    /* Exponentially increase high until it satisfies the inequality.   */
    while ( ((__int128)high * (high - 1)) / 2 < pos )
        high <<= 1;    /* multiply high by 2 */

    /* Binary search on [low, high].                                     */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if ( ((__int128)mid * (mid - 1)) / 2 >= pos )
            high = mid;
        else
            low = mid + 1;
    }
    return low;          /* the desired row number m */
}

/* Main function computing a(n). */
seq_t
a(idx_t n)
{
    if (n < 2)   /* the sequence is defined for n ≥ 2 */
        return 0;

    idx_t pos = n - 1;                       /* position in flattened triangle */
    idx_t m   = find_row(pos);               /* row number */

    /* Number of terms before this row (0‑based).                       */
    idx_t prev = ((m - 1) * (m - 2)) / 2;

    idx_t offset = pos - prev;                /* 1‑based offset in the row   */
    idx_t k = 1 + offset;                     /* corresponding base (2 ≤ k ≤ m) */

    /* Compute the sum of digits of m in base k.                        */
    seq_t value = (seq_t)m;
    seq_t base  = (seq_t)k;
    seq_t sum   = 0;

    while (value > 0) {
        sum += value % base;
        value /= base;
    }

    return sum;
}
