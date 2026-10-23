/*
 * A115598  Consider all Pythagorean triples (X,X+1,Z) ordered by increasing
 *          Z; sequence gives Z-(X+1) values.
 * https://oeis.org/A115598
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1)-7*a(n-2)+a(n-3) Initial values: a(1)=1, a(2)=8,
 *            a(3)=49 Sequence A115598, OEIS
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 25, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first three terms directly. */
    if (n == 1) return 1;
    if (n == 2) return 8;
    if (n == 3) return 49;

    /* Sliding window for the recurrence:
       a_prev3 = a(n-3), a_prev2 = a(n-2), a_prev1 = a(n-1)   */
    seq_t a_prev3 = 1;   /* a(1) */
    seq_t a_prev2 = 8;   /* a(2) */
    seq_t a_prev1 = 49;  /* a(3) */

    /* Compute terms up to the requested n. */
    for (idx_t i = 4; i <= n; ++i) {
        seq_t diff = a_prev1 - a_prev2;        /* a(n-1) - a(n-2)  */
        seq_t new_val = diff * 7 + a_prev3;    /* 7*(a(n-1)-a(n-2)) + a(n-3) */
        /* Shift the window forward. */
        a_prev3 = a_prev2;
        a_prev2 = a_prev1;
        a_prev1 = new_val;
    }

    return a_prev1;
}
