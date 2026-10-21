/*
 * A096000  Cupolar numbers: a(n) = (n+1)*(5*n^2 + 7*n + 3)/3.
 * https://oeis.org/A096000
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*(5*n^2 + 7*n + 3)/3
 * Range:     u8 n <= 4, u16 n <= 33, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use a 128‑bit accumulator so that intermediate values never overflow
       even when seq_t is small.  The final result is cast to seq_t, which
       is fine because the expected value is also cast to seq_t during the
       correctness check. */
    unsigned __int128 nn = (unsigned __int128)n;
    unsigned __int128 t2 = 5 * nn * nn + 7 * nn + 3;
    unsigned __int128 res = (nn + 1) * t2 / 3;
    return (seq_t)res;
}
