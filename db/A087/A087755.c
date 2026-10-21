/*
 * A087755  Triangle read by rows: Stirling numbers of the first kind
 *          (A008275) mod 2.
 * https://oeis.org/A087755
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for the parity triangle of Stirling numbers of the first kind.
 * T(1,1)=T(2,1)=T(2,2)=1.
 * For n ≥ 3:
 *   if n is odd : T(n,k) = T(n-1,k-1)
 *   if n is even: T(n,k) = T(n-1,k-1) XOR T(n-1,k)
 * a(n) is the element at index n when this triangle is read row by row. */

#include <stdlib.h>

static seq_t *seq_cache = NULL;      /* flattened triangle values */
static size_t seq_cache_size = 0;     /* number of values stored    */
static size_t seq_cache_cap  = 0;     /* current capacity of cache  */

static seq_t *prev_row = NULL;       /* previous row data          */
static size_t prev_len  = 0;          /* length of prev_row         */
static idx_t  row_count = 0;          /* number of rows computed    */

/* Ensure that rows up to 'target_row' (inclusive) are computed and stored. */
static void ensure_to_row(idx_t target_row)
{
    while (row_count < target_row) {
        idx_t new_len = row_count + 1;          /* length of the new row    */
        seq_t *new_row = (seq_t*)malloc(new_len * sizeof(seq_t));
        if (!new_row) exit(1);                 /* allocation failure      */

        if (row_count == 0) {                  /* row 1 */
            new_row[0] = 1;
        } else {
            if (new_len & 1) { /* odd row */
                new_row[0] = 0;
                for (idx_t k = 2; k <= new_len; ++k)
                    new_row[k-1] = prev_row[k-2];
            } else {          /* even row */
                for (idx_t k = 1; k <= new_len; ++k) {
                    seq_t a = (k >= 2) ? prev_row[k-2] : 0;
                    seq_t b = (k-1 < prev_len) ? prev_row[k-1] : 0;
                    new_row[k-1] = a ^ b;
                }
            }
        }

        /* Append the new row to the flattened cache */
        size_t required = seq_cache_size + new_len;
        if (required > seq_cache_cap) {
            size_t new_cap = seq_cache_cap ? seq_cache_cap * 2 : 128;
            while (new_cap < required) new_cap *= 2;
            seq_t *tmp = (seq_t*)realloc(seq_cache, new_cap * sizeof(seq_t));
            if (!tmp) exit(1);
            seq_cache = tmp;
            seq_cache_cap = new_cap;
        }
        for (idx_t i = 0; i < new_len; ++i)
            seq_cache[seq_cache_size + i] = new_row[i];
        seq_cache_size += new_len;

        /* Prepare for the next iteration */
        free(prev_row);
        prev_row = new_row;
        prev_len = new_len;
        row_count = new_len;
    }
}

seq_t a(idx_t n)
{
    /* Find the row that contains the n-th element (rows are 1‑based). */
    idx_t r = 1, tri = 0;
    while (tri + r < n) { tri += r; ++r; }

    ensure_to_row(r);
    return seq_cache[n - 1];
}
