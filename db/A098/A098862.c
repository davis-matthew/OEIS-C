/*
 * A098862  a(n) = column number of Stolarsky array that contains n.
 * https://oeis.org/A098862
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Stolarsky array – new column x starts at position
 *            Fibonacci(x+2); between the first x and the first x+1 the values
 *            are the reversal of all entries before the first (x‑1). This
 *            generates the required sequence
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* undefined offset, return 0 */

    /* Allocate array for positions 1..n (index 0 unused) */
    seq_t *arr = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!arr) return (seq_t)0;      /* allocation failure */

    arr[1] = (seq_t)0;              /* a(1) = 0 */

    /* Pre‑compute Fibonacci numbers up to > n+something */
    idx_t fib[64];
    fib[0] = 0;
    fib[1] = 1;
    size_t fib_len = 2;
    while (fib[fib_len - 1] <= n + 1 && fib_len < 64) {
        fib[fib_len] = fib[fib_len - 1] + fib[fib_len - 2];
        fib_len++;
    }

    size_t cur_col = 0;             /* current column index */
    while (1) {
        /* Position of the first occurrence of the next column */
        idx_t start = fib[cur_col + 3];   /* first[x+1] where x = cur_col */
        if (start > n) break;             /* no more new columns to process */

        /* Assign the new column value */
        arr[(size_t)start] = (seq_t)(cur_col + 1);

        /* End of the segment belonging to this new column */
        idx_t next_start = fib[cur_col + 4];           /* first[x+2] */
        idx_t seg_end = (next_start == 0 || next_start == 1) ? n
                                                              : (next_start - 1);
        if (seg_end > n) seg_end = n;

        /* Length of the source segment (prefix before the previous column) */
        idx_t src_len = fib[cur_col + 2] - 1;          /* first[x] - 1 */

        /* Copy the reversed prefix into the new segment */
        size_t copy_start = (size_t)(start + 1);
        size_t copy_end   = (size_t)seg_end;
        if (copy_end > n) copy_end = n;
        if (copy_start <= copy_end) {
            size_t copy_len = copy_end - copy_start + 1;
            for (size_t i = 0; i < copy_len; ++i) {
                /* src index is src_len - i, guaranteed to be >= 1 */
                arr[copy_start + i] = arr[(size_t)(src_len - (idx_t)i)];
            }
        }

        cur_col++;  /* move to the next column */
    }

    seq_t result = arr[(size_t)n];
    free(arr);
    return result;
}
