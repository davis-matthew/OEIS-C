/*
 * A066237  First differences give A052849.
 * https://oeis.org/A066237
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = -1, a(1) = 1, a(n) = n*a(n-1) - (n-1)*a(n-2) for n ≥ 2;
 *            Offset 1: a(1) = 1 is the first term listed in OEIS
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Handle the trivial cases */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;  /* computed from recurrence with a(0) = -1 */

    /* Iterative computation using the recurrence.
       We use seq_t for all arithmetic to avoid overflow and support
       arbitrary-precision builds. */
    seq_t a_prevprev = (seq_t)1;   /* a(1) */
    seq_t a_prev    = (seq_t)3;   /* a(2) */
    seq_t a_curr;

    for (idx_t i = 3; i <= n; ++i) {
        a_curr = (seq_t)i * a_prev - (seq_t)(i - 1) * a_prevprev;
        a_prevprev = a_prev;
        a_prev = a_curr;
    }
    return a_curr;
}
