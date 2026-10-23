/*
 * A168522  a(n) = 98*a(n-1) - 2*a(n-2); a(1) = 0, a(2) = 1.
 * https://oeis.org/A168522
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 98*a(n-1) - 2*a(n-2); a(1) = 0, a(2) = 1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS A168522.  Works for any order of calls
 * but does not memoize; a linear recurrence of order two requires only
 * O(n) arithmetic operations, which is fast enough for n up to ~10 000. */
seq_t a(idx_t n)
{
    /* The offset is 1; values for n < 1 are undefined but we return 0. */
    if (n < 2) {
        /* n == 1 -> a(1) = 0,  n == 0 (or negative) -> arbitrary */
        return (seq_t)0;
    }
    if (n == 2) {
        return (seq_t)1;
    }

    /* Iterative evaluation of the linear recurrence. */
    seq_t prev2 = (seq_t)0;      /* a(1) */
    seq_t prev1 = (seq_t)1;      /* a(2) */
    seq_t curr  = (seq_t)0;      /* placeholder for a(i) */

    for (idx_t i = 3; i <= n; ++i) {
        /* a(i) = 98*a(i-1) - 2*a(i-2) */
        curr = (seq_t)98 * prev1 - (seq_t)2 * prev2;
        prev2 = prev1;
        prev1 = curr;
    }

    return curr;   /* a(n) */
}
