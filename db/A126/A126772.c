/*
 * A126772  Padovan factorials: a(n) is the product of the first n terms of
 *          the Padovan sequence. Similar to the Fibonacci factorial.
 * https://oeis.org/A126772
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∏_{k=1}^{n} P(k) where P(0)=P(1)=P(2)=1 and
 *            P(k)=P(k-2)+P(k-3). The function returns the product of the
 *            first n Padovan numbers (offset 1)
 * Range:     u8 n <= 8, u16 n <= 10, u32 n <= 14, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;          /* a(1) = 1 */

    /* Padovan values grow as P(0)=P(1)=P(2)=1.  We store the last
       three values to compute the next one. */
    seq_t p1 = (seq_t)1; /* P(i-3) */
    seq_t p2 = (seq_t)1; /* P(i-2) */
    seq_t p3 = (seq_t)1; /* P(i-1) */

    /* First three Padovan numbers are all 1, so the product is 1. */
    seq_t prod = (seq_t)1;

    /* Compute P(4) .. P(n) and multiply the running product. */
    for (idx_t i = 4; i <= n; ++i) {
        seq_t next = p1 + p2;   /* P(i) = P(i-2) + P(i-3) */
        prod = prod * next;
        /* Shift the window for the next iteration. */
        p1 = p2;
        p2 = p3;
        p3 = next;
    }

    return prod;
}
