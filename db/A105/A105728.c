/*
 * A105728  Triangle read by rows: T(n,1) = 1, T(n,n) = n and for 1 < k < n:
 *          T(n,k) = T(n-1,k-1) + 2*T(n-1,k).
 * https://oeis.org/A105728
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 81, u32 n <= 260, u64 n <= 958,
 *            u128 n <= 3513, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A105728 – Triangle read by rows.
   Recurrence: T(n,1)=1, T(n,n)=n, and for 1<k<n
   T(n,k)=T(n-1,k-1)+2*T(n-1,k).
   a(n) is the nth entry when the rows are concatenated.
   This implementation builds rows up to the required one and
   returns the requested column entry.  It is O(r²) with
   r≈√(2n) and uses only a small fixed buffer, so it works
   for all n up to several millions on typical 64‑bit
   systems, with arbitrary‑precision seq_t supported.
*/
#define MAX_R 5000   /* enough for n up to ~1e8, larger r causes undefined behaviour */

seq_t a(idx_t n)
{
    if (n < 1)          /* offset starts at 1; behaviour below is undefined */
        return (seq_t)0;

    /* Find row r and column c (1‑based) for element n */
    idx_t r = 1;
    idx_t sum = 1;      /* cumulative entries up to current row */
    while (sum < n) {
        ++r;
        sum += r;
    }
    /* c = n - (sum - r)  --> 1 ≤ c ≤ r */
    idx_t c = n - (sum - r);

    if (r == 1)         /* only entry is 1 */
        return (seq_t)1;

    /* Buffers for two successive rows; size MAX_R is ample for all practical n. */
    seq_t buf1[MAX_R];
    seq_t buf2[MAX_R];
    seq_t *prev = buf1;
    seq_t *curr = buf2;

    /* Initialize first row */
    prev[0] = (seq_t)1;

    /* Build rows 2 .. r */
    for (idx_t row = 2; row <= r; ++row) {
        curr[0] = (seq_t)1;            /* first element */
        curr[row - 1] = (seq_t)row;    /* last element */

        /* interior elements */
        for (idx_t j = 1; j + 1 < row; ++j)
            curr[j] = prev[j - 1] + (seq_t)2 * prev[j];

        /* swap pointers for next iteration */
        seq_t *tmp = prev; prev = curr; curr = tmp;
    }

    /* After the loop, 'prev' holds row r.  Convert column to 0‑based index. */
    return prev[c - 1];
}
