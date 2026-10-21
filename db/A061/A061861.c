/*
 * A061861  First two significant digits of 1/n written in decimal.
 * https://oeis.org/A061861
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = floor(10^(floor(2+log10(n-1)))/n), a(1) = 10
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Returns the first two significant digits of 1/n written in decimal. */
seq_t a(idx_t n)
{
    /* The offset: a(1) = 10 */
    if (n == 1)
        return (seq_t)10;

    /* n ≥ 2 : work with the unsigned 64‑bit representation of the index   */
    idx_t m = n - 1;          /* m > 0 */

    /* Count decimal digits of m :  d = ⌊log10(m)⌋ + 1  */
    idx_t d = 0;
    idx_t tmp = m;
    do {
        ++d;
        tmp /= 10;
    } while (tmp != 0);

    /* k = ⌊log10(m)⌋ + 2 = d + 1  */
    idx_t k = d + 1;

    /* Compute 10^k as a 64‑bit unsigned integer (safe for the limits used) */
    uint64_t pow10 = 1;
    for (idx_t i = 0; i < k; ++i)
        pow10 *= 10ULL;

    /* The result fits into seq_t because 10^k / n is always between 10 and 99 */
    uint64_t res = pow10 / (uint64_t)n;
    return (seq_t)res;
}
