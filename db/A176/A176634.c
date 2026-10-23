/*
 * A176634  a(n) = 6*a(n-1)-8*a(n-2)-3 for n > 2; a(0) = 89, a(1) = 519, a(2)
 *          = 2063.
 * https://oeis.org/A176634
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 4, u32 n <= 12, u64 n <= 28, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = 6*a(n-1) - 8*a(n-2) - 3 for n > 2;
   a(0) = 89, a(1) = 519, a(2) = 2063
   (OEIS A176634) */
seq_t a(idx_t n)
{
    /* handle the explicitly given initial terms */
    if (n == 0) {
        return (seq_t)89;
    }
    if (n == 1) {
        return (seq_t)519;
    }
    if (n == 2) {
        return (seq_t)2063;
    }

    /* iteratively compute from a(1) and a(2) up to a(n) */
    seq_t prev2 = (seq_t)519;   /* a(n-2) */
    seq_t prev1 = (seq_t)2063;  /* a(n-1) */
    seq_t curr = 0;

    /* loop from n = 3 to the requested n */
    for (idx_t i = 3; i <= n; ++i) {
        /* recurrence: a(i) = 6*a(i-1) - 8*a(i-2) - 3 */
        curr = (seq_t)6 * prev1 - (seq_t)8 * prev2 - (seq_t)3;
        /* shift for next iteration */
        prev2 = prev1;
        prev1 = curr;
    }
    return curr;
}
