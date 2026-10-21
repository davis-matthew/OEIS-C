/*
 * A091692  a(n) = (10^n-1) * (n+9) / 9.
 * https://oeis.org/A091692
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (10^n - 1) * (n + 9) / 9 which equals repunit(n) * (n +
 *            9)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* repunit(n) is the number consisting of n ones. */
/* a(0) = 0, a(1) = 10, a(2) = 121, ... as in OEIS A091692. */
seq_t a(idx_t n)
{
    seq_t r = 0;           /* repunit(n) */
    for (idx_t i = 0; i < n; ++i)
        r = r * 10 + 1;    /* build the repunit iteratively */
    return r * (seq_t)(n + 9);
}
