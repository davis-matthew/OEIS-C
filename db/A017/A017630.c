/*
 * A017630  a(n) = (12*n + 9)^2.
 * https://oeis.org/A017630
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (12*n + 9)^2
 * Range:     u8 n <= 0, u16 n <= 20, u32 n <= 5460, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast index and constants to seq_t to keep arithmetic within the
       representation type, allowing natural wrap‑around for unsigned
       arithmetic. */
    seq_t nn = (seq_t)n;
    seq_t val = (seq_t)12 * nn + (seq_t)9;
    return val * val;
}
