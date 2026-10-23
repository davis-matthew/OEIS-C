/*
 * A152524  a(n) is the number of L-bit words in which, if up to k bits are
 *          perturbed, the resulting change in unsigned L-bit value is n, for
 *          L=8 and k=7.
 * https://oeis.org/A152524
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 511 + (-1)^n - 2*n
 * Range:     u8 none, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use a signed 64‑bit intermediate to avoid overflow and negative
       wrap‑around.  For n > 255 the formula would become negative, so
       clamp those results to 0. */
    long long delta = (n & 1ULL) ? -1LL : 1LL;
    long long val   = 511 + delta - 2LL * (long long)n;
    if (val < 0) val = 0;
    return (seq_t)val;
}
