/*
 * A046934  Same rule as Aitken triangle (A011971) except a(0,0)=1, a(1,0)=0.
 * https://oeis.org/A046934
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabl, easy, nice, nonn
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 56, u32 n <= 136, u64 n <= 349,
 *            u128 n <= 952, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS Sequence A046934
 *
 *  The table is built by the same recurrence as the Aitken triangle:
 *
 *          a(i,0) = 0  for i >= 1    (except a(0,0)=1)
 *          a(i,j) = a(i-1,j) + a(i-1,j-1)   for j >= 1
 *
 *  The flattened list of the rows (read left‑to‑right, top‑to‑bottom)
 *  gives the sequence a(n) with offset 0:  a(0) = 1, a(1) = 0, a(2) = 1, ….
 *
 *  The function stores computed rows in a dynamic array and expands the
 *  array as needed.  All arithmetic is performed in seq_t, and the
 *  allocation routine uses a C++‑style cast to avoid a compilation error
 *  when seq_t is an arbitrary‑precision class.
 *
 *  Complexity:  O(n) time, O(n) memory for n terms.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *data   = NULL;   /* flattened sequence values      */
static size_t capacity = 0;    /* allocated capacity in seq_t    */
static size_t size     = 0;    /* number of values currently stored (>= 1 when initialized) */
static idx_t  max_row  = 0;    /* highest row index computed (0‑based) */

/* Ensure the internal array can store at least 'needed' items. */
static void ensure_capacity(size_t needed)
{
    if (needed <= capacity) return;
    size_t newcap = capacity ? capacity << 1 : 64;
    while (newcap < needed) newcap <<= 1;
    seq_t *tmp = (seq_t *)realloc(data, newcap * sizeof *data);
    data = tmp;
    capacity = newcap;
}

/* Return the n-th term of A046934. */
seq_t a(idx_t n)
{
    /* Initialise on first call. */
    if (data == NULL)
    {
        ensure_capacity(128);
        data[0] = (seq_t)1;   /* row 0 */
        size    = 1;
        max_row = 0;
    }

    /* Grow the table until we have at least n+1 terms. */
    while ((idx_t)size <= n)
    {
        idx_t r = max_row + 1;          /* index of new row   */

        if (r == 1)                     /* special handling of row 1 */
        {
            /* Append 0, 1 */
            ensure_capacity(size + 2);
            data[size]   = (seq_t)0;
            data[size+1] = (seq_t)1;
            size  += 2;
            max_row = 1;
            continue;
        }

        /* For r >= 2: previous row length = r */
        size_t prev_len    = (size_t)r;
        size_t prev_start  = size - prev_len;   /* start index of previous row */
        seq_t  prev_last   = data[size - 1];     /* last element of previous row */

        /* Reserve space for the new row of length r+1 */
        ensure_capacity(size + (size_t)(r + 1));

        size_t start_new = size;               /* where new row begins   */
        data[start_new] = prev_last;           /* a_r[0]  */

        /* Compute remaining elements of the new row */
        for (idx_t k = 1; k <= r; ++k)
        {
            data[start_new + (size_t)k] =
                data[start_new + (size_t)(k - 1)]     /* a_r[k-1]   */
              + data[prev_start + (size_t)(k - 1)];     /* a_{r-1}[k-1] */
        }

        size   += (size_t)(r + 1);
        max_row = r;
    }

    return data[(size_t)n];
}
