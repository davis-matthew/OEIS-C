/*
 * A094294  a(n) = n*a(n-1) - n + 2 for n > 1; a(1)=1.
 * https://oeis.org/A094294
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = n*a(n-1) - n + 2; a(1) = 1 (offset = 1)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* values below the offset are not defined */

    seq_t res = 1;                /* a(1) */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t ii = (seq_t)i;
        res = res * ii - ii + 2;  /* a(i) = i*a(i-1) - i + 2 */
    }
    return res;
}
