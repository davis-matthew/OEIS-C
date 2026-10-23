/*
 * A143096  a(n) = 2*a(n-1)-1, with a(1)=1, a(2)=4, a(3)=5.
 * https://oeis.org/A143096
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1, a(2)=4; for n≥3, a(n)=1+2^(n‑1) (equivalently
 *            a(n)=2*a(n‑1)‑1)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the fixed initial terms. */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)4;

    /* For n≥3 we can start from a(3)=5 and apply the recurrence a(n)=2*a(n‑1)‑1. */
    seq_t val = (seq_t)5;          /* a(3) */
    for (idx_t i = 4; i <= n; ++i)
    {
        val = (val << 1) - 1;      /* val = 2*val – 1 */
    }
    return val;
}
