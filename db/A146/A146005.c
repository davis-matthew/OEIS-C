/*
 * A146005  a(n) = n*Lucas(n).
 * https://oeis.org/A146005
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n * Lucas(n) (Lucas(0)=2, Lucas(1)=1)
 * Range:     u8 n <= 7, u16 n <= 17, u32 n <= 38, u64 n <= 83, u128 n <= 173,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first two trivial cases */
    if (n == 0u) return 0;
    if (n == 1u) return (seq_t)n * 1;   /* Lucas(1) = 1, 1*1 = 1 */

    /* Compute Lucas numbers iteratively, using only two temporaries */
    seq_t l0 = 2;          /* Lucas(0) */
    seq_t l1 = 1;          /* Lucas(1) */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t l = l1 + l0;  /* Lucas(i) = Lucas(i-1) + Lucas(i-2) */
        l0 = l1;
        l1 = l;
    }

    /* Now l1 holds Lucas(n) */
    return ((seq_t)n) * l1;
}
