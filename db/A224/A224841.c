/*
 * A224841  Triangle T(n,k) read by rows: Substring of k digits of sequence
 *          A007376, ending at position n, 1 <= k <= n.
 * https://oeis.org/A224841
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base, tabl
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 65, u64 n <= 229, u128 n <= 819,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A224841:  T(n,k) = k‑digit substring of the decimal expansion of A007376
 *  ending at position n, read by rows (row 1, then row 2, …)               */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* external linkage --------------------------------------------------------*/
seq_t a(idx_t n)
{
    /* Find the row r (1‑based) and column k (1‑based) that correspond to
     * the global index n in the triangle.  The number of terms up to row
     * r is r*(r+1)/2. */
    idx_t lo = 0, hi = 60000;                 /* sufficient for n≈10⁴ */
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        idx_t t   = mid * (mid + 1) / 2;
        if (t >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t row = lo;                           /* triangle row (1‑based) */
    idx_t before = (row - 1) * row / 2;       /* terms before this row   */
    idx_t k = n - before;                     /* column index (1‑based)  */

    /* ---------------------------------------------------------------------*/
    /* Build the decimal expansion of A007376 up to the required position. */
    /* ---------------------------------------------------------------------*/
    static char *digits_buf  = NULL;          /* buffer for digits */
    static size_t digits_cap = 0;             /* allocated size      */
    static idx_t   digits_len = 0;            /* number of chars used*/
    static uint64_t next_num = 1;             /* next integer to write */

    if (digits_len < row) {
        while (digits_len < row) {
            char tmp[32];
            int len = snprintf(tmp, sizeof(tmp), "%llu",
                               (unsigned long long)next_num);
            if (digits_len + (size_t)len > digits_cap) {
                size_t new_cap = digits_cap ? digits_cap * 2 : 128;
                while (digits_len + (size_t)len > new_cap)
                    new_cap *= 2;
                char *new_buf = (char*)realloc(digits_buf, new_cap);
                if (!new_buf) abort();
                digits_buf = new_buf;
                digits_cap = new_cap;
            }
            memcpy(digits_buf + digits_len, tmp, (size_t)len);
            digits_len += (size_t)len;
            ++next_num;
        }
    }

    /* ---------------------------------------------------------------------*/
    /* Extract the k‑digit substring ending at position row.               */
    /* ---------------------------------------------------------------------*/
    idx_t start = row - k;                    /* inclusive, 0‑based     */
    seq_t val = 0;
    for (idx_t i = start; i < row; ++i)
        val = val * 10 + (seq_t)(digits_buf[i] - '0');

    return val;
}
