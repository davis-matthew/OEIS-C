/*
 * A235757  Ruler function associated with the set of permutations generated
 *          by cyclic shift in cyclic order, array read by rows.
 * https://oeis.org/A235757
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 93, u16 n <= 93, u32 n <= 93, u64 n <= 93, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  F(2) = [1,1].
   For n>2, F(n) is obtained from F(n-1) by
   1) incrementing every entry of F(n-1) by 1 (call the result F'(n-1));
   2) inserting a run of n-1 ones before the first entry of F'(n-1) and
      between consecutive entries of F'(n-1).
   The sequence a is the concatenation of all rows F(n), indexed from a(2). */

#include <stdint.h>

/* factorials[0] = 0! = 1.  factorials[i] = i!  for 1 ≤ i ≤ 20. */
static idx_t factorials[21];
static void init_factorials(void) {
    static int initialized = 0;
    if (!initialized) {
        factorials[0] = 1;
        for (idx_t i = 1; i <= 20; ++i)
            factorials[i] = factorials[i-1] * i;
        initialized = 1;
    }
}

/* Return the value of the p-th element (0‑based) of row n. */
static seq_t row_value(idx_t n, idx_t p) {
    if (n == 2) return (seq_t)1;          /* base row */

    idx_t k = n - 1;                      /* length of inserted runs of ones */

    if (p < k) return (seq_t)1;           /* leading run of ones */

    idx_t m = factorials[n-1];            /* (n-1)!  = length of F(n-1) */
    idx_t internal = p - k;               /* index inside the body */
    idx_t blockCount = m - 1;             /* blocks that contain a value + k ones */
    idx_t blockSize  = k + 1;             /* length of each such block */

    if (internal >= blockCount * blockSize) {   /* last block – only the value */
        idx_t prev = blockCount;                /* index m-1 in the previous row */
        return row_value(n-1, prev) + (seq_t)1;
    }

    idx_t block  = internal / blockSize;        /* block containing the requested entry */
    idx_t offset = internal % blockSize;        /* position within that block */

    if (offset == 0)                            /* the value entry itself */
        return row_value(n-1, block) + (seq_t)1;

    return (seq_t)1;                            /* one of the inserted ones */
}

/* Main function required by the interface. */
seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;        /* defined only for n ≥ 2 */

    init_factorials();

    idx_t cumulative = 0;               /* total length of all preceding rows */
    for (idx_t r = 2; r <= 20; ++r) {
        idx_t fact = factorials[r];      /* r!  = length of row r */
        if (n <= cumulative + 1 + fact) {
            idx_t p = n - (cumulative + 2);   /* 0‑based position in row r */
            return row_value(r, p);
        }
        cumulative += fact;
    }
    return (seq_t)0;                     /* should never happen for valid indices */
}
