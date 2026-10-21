/*
 * A071761  Dealing cards in a game of solitaire.
 * https://oeis.org/A071761
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   * a(0) = 0 * For n > 0: * Let m = smallest integer with m(m+1)/2
 *            ≥ n * Let k = n - (m-1)m/2 - 1 (0‑based offset inside block) *
 *            If k < m-1, omit the digit (m-1-k) from the decreasing * list
 *            m-1, m-2, …, 1; otherwise include the entire list. * The
 *            resulting number is formed by concatenating m and...
 * Range:     u8 n <= 5, u16 n <= 20, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;
    }

    /* Find block index m such that m(m+1)/2 ≥ n */
    idx_t m = 1;
    while ((m * (m + 1)) / 2 < n) {
        ++m;
    }

    /* Offset within the block: 0 … m-1 */
    idx_t start  = (m - 1) * m / 2;
    idx_t offset = n - start - 1;

    /* Digit to skip: 0 means no skip */
    idx_t skip_digit = (offset == m - 1) ? 0 : (m - 1 - offset);

    /* Build the result by appending digits */
    seq_t value = (seq_t)m;
    for (idx_t d = m - 1; d > 0; --d) {
        if (d == skip_digit) {
            continue;               /* omit this digit */
        }
        value = value * (seq_t)10 + (seq_t)d;
    }

    return value;
}
