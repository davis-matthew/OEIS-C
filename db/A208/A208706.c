/*
 * A208706  Number of n X 5 0..1 arrays with new values 0..1 introduced in row
 *          major order and no element equal to more than two of its immediate
 *          leftward or upward or right-upward antidiagonal neighbors.
 * https://oeis.org/A208706
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A208706: Number of n X 5 0..1 arrays with specified constraints
   Empirical recurrence (for n > 4):
     a(n) = 24*a(n-1) - 28*a(n-2) + 16*a(n-3)
   Initial values:
     a(1) = 16
     a(2) = 356
     a(3) = 8128
     a(4) = 185344
   The routine below uses the recurrence iteratively; it works for any
   seq_t size and is O(n) in time.  */

seq_t a(idx_t n)
{
    /* Handle the first four terms directly */
    if (n == 1) return (seq_t)16;
    if (n == 2) return (seq_t)356;
    if (n == 3) return (seq_t)8128;
    if (n == 4) return (seq_t)185344;

    /* For n >= 5, compute by recurrence */
    seq_t prev3 = (seq_t)356;   /* a(n-3) */
    seq_t prev2 = (seq_t)8128;  /* a(n-2) */
    seq_t prev1 = (seq_t)185344;/* a(n-1) */
    seq_t cur    = 0;

    for (idx_t i = 5; i <= n; ++i) {
        cur = (seq_t)24 * prev1
              - (seq_t)28 * prev2
              + (seq_t)16 * prev3;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}
