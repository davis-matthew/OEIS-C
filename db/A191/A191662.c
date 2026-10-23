/*
 * A191662  a(n) = n! / A000034(n-1).
 * https://oeis.org/A191662
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n! / A000034(n-1) Recurrence: a(1) = a(2) = 1, a(n) =
 *            (n-1)*n*a(n-2) for n ≥ 3 (OEIS A191662)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* the sequence is defined for n ≥ 1; for n == 0 we return 0 */
    if (n == 0)   return (seq_t)0;
    if (n == 1)   return (seq_t)1;
    if (n == 2)   return (seq_t)1;

    /* iterate using the two‑term recurrence */
    seq_t a_prevprev = (seq_t)1;   /* a(1) */
    seq_t a_prev     = (seq_t)1;   /* a(2) */

    for (idx_t i = 3; i <= n; ++i) {
        seq_t prod   = (seq_t)(i - 1) * (seq_t)i;   /* (i-1)*i */
        seq_t a_new  = a_prevprev * prod;           /* a(n) = prod * a(n-2) */
        a_prevprev = a_prev;
        a_prev     = a_new;
    }
    return a_prev;   /* holds a(n) after loop completion */
}
