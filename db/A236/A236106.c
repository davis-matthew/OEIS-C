/*
 * A236106  Triangle read by rows: T(n,k), n>=1, k>=1, in which column k lists
 *          the twice odd numbers (A016825) interleaved with k-1 zeros, and
 *          the first element of column k is in row k(k+1)/2.
 * https://oeis.org/A236106
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle read by rows.
 * T(r,k) = 0                                       if r < k(k+1)/2 or (r-k(k+1)/2) % k ≠ 0
 *          4*((r-k(k+1)/2)/k)+2   otherwise
 * a(n) is the n‑th element of the triangle read left–to–right, top–to–bottom. */

#include <stdint.h>   /* for uint64_t etc. */

static idx_t row_len(idx_t r)
{
    /* returns the number of columns that have an entry in row r */
    idx_t k = 0;
    for (;;) {
        unsigned __int128 next = (unsigned __int128)(k + 1) * (k + 2) / 2;
        if (next > r) break;
        k++;
    }
    return k;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for 0, harmless */

    idx_t sum = 0;                /* elements before current row */
    idx_t r = 1;                  /* current row (1‑based) */

    for (;;) {                    /* find the row that contains the n‑th element */
        idx_t len = row_len(r);
        if (sum + len >= n) break;
        sum += len;
        r++;
    }

    idx_t k = n - sum;            /* column index within that row (1‑based) */

    /* first row of column k */
    unsigned __int128 start_row128 = (unsigned __int128)k * (k + 1) / 2;
    idx_t start_row = (idx_t)start_row128;

    if (r < start_row) return 0;  /* should not happen, but keep safe */

    idx_t diff = r - start_row;
    if (diff % k != 0) return 0;  /* zero inserted between entries */

    idx_t j = diff / k;           /* j = 0,1,2,... for successive non‑zero entries */

    seq_t val = (seq_t)j * 4 + 2;  /* 4*j + 2 (twice the odd number sequence) */
    return val;
}
