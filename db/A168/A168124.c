/*
 * A168124  a(n) = n^2*(n^6+1)/2.
 * https://oeis.org/A168124
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2*(n^6+1)/2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 17, u64 n <= 279, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;                /* convert index to seq_t */
    seq_t n2    = n_seq * n_seq;           /* n^2 */
    seq_t n6    = n2 * n2 * n2;            /* n^6 = (n^2)^3 */
    seq_t sum   = n6 + (seq_t)1;           /* n^6 + 1 */

    /* Divide by 2 on an even factor to avoid overflow in 64‑bit builds. */
    if ((n_seq & (seq_t)1) == 0) {
        n2 = n2 >> 1;                     /* n even → n^2 divisible by 2 */
    } else {
        sum = sum >> 1;                   /* n odd  → n^6 + 1 divisible by 2 */
    }

    return n2 * sum;
}
