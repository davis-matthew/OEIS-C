/*
 * A081335  a(n) = (6^n + 2^n)/2.
 * https://oeis.org/A081335
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (6^n + 2^n)/2 Recurrence (used for efficient
 *            computation): a(0)=1, a(1)=4, a(n)=8*a(n-1)-12*a(n-2) for n>=2
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the base cases directly. */
    if (n == (idx_t)0) return (seq_t)1;
    if (n == (idx_t)1) return (seq_t)4;

    /* Iterate the linear recurrence using two rolling variables. */
    seq_t prevprev = (seq_t)1;   /* a(0) */
    seq_t prev     = (seq_t)4;   /* a(1) */
    seq_t curr = 0;

    for (idx_t i = (idx_t)2; i <= n; i++) {
        curr = (seq_t)8 * prev - (seq_t)12 * prevprev;
        prevprev = prev;
        prev = curr;
    }
    return curr;
}
