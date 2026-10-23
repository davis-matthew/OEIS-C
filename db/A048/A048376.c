/*
 * A048376  Replace each 1 in decimal expansion of n with 1 1's, each 2 with 2
 *          2's, etc. (0 vanishes).
 * https://oeis.org/A048376
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base, nice
 * Formula:   Each non‑zero decimal digit d of n is replaced by d copies of d
 *            (0 disappears). Hence a(n) is the integer obtained by this
 *            replication
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 28, u64 n <= 298, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle n == 0, although the offset starts at 1. */
    if (n == 0)
        return (seq_t)0;

    /* Store decimal digits of n in reverse order. */
    idx_t digits[20];
    idx_t len = 0;
    while (n > 0) {
        digits[len++] = n % 10;
        n /= 10;
    }

    /* Build the result by processing digits from most significant to least. */
    seq_t result = 0;
    for (idx_t i = len; i > 0; --i) {
        idx_t d = digits[i - 1];
        if (d == 0)
            continue;               /* 0 vanishes */
        for (idx_t r = 0; r < d; ++r) {
            result = result * 10 + d;    /* append digit d */
        }
    }
    return result;
}
