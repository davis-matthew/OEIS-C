/*
 * A077753  a(0) = 2, a(1) = 1, a(2n) = a(2n-1) * a(2n-2), a(2n+1) = a(2n-1) +
 *          a(2n).
 * https://oeis.org/A077753
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 9, u32 n <= 11, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Recurrence:  a(0) = 2,  a(1) = 1
    For n ≥ 2
        if n is even   a(n) = a(n‑1) * a(n‑2)
        if n is odd    a(n) = a(n‑1) + a(n‑2)
    The implementation is order‑independent and uses only O(1) memory. */
seq_t a(idx_t n)
{
    /* Handle the two initial values explicitly */
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)1;

    /* prev2 = a(i‑2),  prev1 = a(i‑1) ; start with i = 2 */
    seq_t prev2 = (seq_t)2;   /* a(0) */
    seq_t prev1 = (seq_t)1;   /* a(1) */
    seq_t cur;

    /* Compute from 2 up to n */
    for (idx_t i = 2; i <= n; ++i) {
        if ( (i & (idx_t)1) == 0 ) {           /* even i  -> multiply   */
            cur = prev1 * prev2;
        } else {                                /* odd i   -> add        */
            cur = prev1 + prev2;
        }
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
