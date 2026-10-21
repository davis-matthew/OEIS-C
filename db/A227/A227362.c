/*
 * A227362  Distinct digits of n arranged in decreasing order.
 * https://oeis.org/A227362
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = distinct digits of n arranged in decreasing order
 * Range:     u8 n <= 102, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* If n is zero, the only digit is 0, so return 0 immediately. */
    if (n == 0) return 0;

    /* Build a 10‑bit mask indicating which digits appear in n. */
    unsigned mask = 0;
    idx_t tmp = n;
    while (tmp > 0) {
        unsigned digit = (unsigned)(tmp % 10);
        mask |= 1u << digit;
        tmp /= 10;
    }

    /* Reconstruct the number by taking digits from 9 down to 0. */
    seq_t result = 0;
    for (int d = 9; d >= 0; --d) {
        if (mask & (1u << d)) {
            result = result * 10 + d;
        }
    }
    return result;
}
