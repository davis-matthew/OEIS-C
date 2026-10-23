/*
 * A123264  Pascal's reversal triangle, read by rows.
 * https://oeis.org/A123264
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn, tabl
 * Formula:   a(n) is the row‑wise reading of the Pascal’s reversal triangle.
 *            * The triangle is defined by: * T(0,1)=1; * T(1,1)=T(1,2)=1; *
 *            for i>1, T(i,j) = rev( T(i-1,j-1) + T(i-1,j) ), * where rev(x)
 *            is the decimal digit reversal of x. * The sequence outputs
 *            T(i,j) for i=0,1,2,… and j=1…i+1, concatenated b...
 * Range:     u8 n <= 48, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

static seq_t rev_decimal(seq_t x)
{
    seq_t rev = 0;
    while (x > 0) {
        rev = rev * 10 + (x % 10);
        x /= 10;
    }
    return rev;
}

seq_t a(idx_t n)
{
    /* For n up to 10,000 the maximum row length is 142.
       Allocate a little more to be safe. */
    seq_t row[2048];
    seq_t next[2048];
    seq_t *cur = row;
    seq_t *nxt = next;
    size_t cur_len = 1;      /* number of elements in the current row */
    cur[0] = 1;

    idx_t cumulative = 0;
    idx_t target = n;

    /* Iterate rows until we locate the target position. */
    for (size_t row_idx = 0; ; ++row_idx) {
        cumulative += cur_len;
        if (target <= cumulative) {
            size_t offset = (size_t)(target - (cumulative - cur_len) - 1);
            return cur[offset];
        }

        /* Build the next row, which has cur_len+1 elements. */
        for (size_t k = 0; k <= cur_len; ++k) {
            seq_t left  = (k > 0)   ? cur[k - 1] : 0;
            seq_t right = (k < cur_len) ? cur[k] : 0;
            nxt[k] = rev_decimal(left + right);
        }

        /* Swap the arrays and update length for the next iteration. */
        seq_t *tmp = cur;
        cur = nxt;
        nxt = tmp;
        cur_len++;   /* new row length: previous length + 1 */
    }
}
