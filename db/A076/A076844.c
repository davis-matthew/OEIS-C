/*
 * A076844  a(1) = a(2) = a(3) = 1; a(n) = (a(n-1) + a(n-2) + 1)/a(n-3) (for
 *          n>3).
 * https://oeis.org/A076844
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) defined by a(1)=a(2)=a(3)=1; a(n) =
 *            (a(n-1)+a(n-2)+1)/a(n-3) for n>3. This sequence is periodic with
 *            period 8: [1,1,1,3,5,9,5,3]
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Periodic values of the sequence.  These fit in any seq_t. */
    static const seq_t period[8] = {
        (seq_t)1, (seq_t)1, (seq_t)1,
        (seq_t)3, (seq_t)5, (seq_t)9,
        (seq_t)5, (seq_t)3
    };
    /* Return the term corresponding to n.  Offset is 1. */
    return period[(n - 1) % 8];
}
