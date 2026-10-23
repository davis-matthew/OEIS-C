/*
 * A194459  Number of entries in the n-th row of Pascal's triangle not
 *          divisible by 5.
 * https://oeis.org/A194459
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = ∏_{d=1}^{4} (d+1)^{b(n,d)} (b(n,d) = number of digits d
 *            in the base‑5 expansion of n)
 * Range:     u8 n <= 348, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = (seq_t)1;
    idx_t t = n;
    while (t) {
        idx_t digit = t % 5;
        if (digit > 0) {
            result *= (seq_t)(digit + 1);
        }
        t /= 5;
    }
    return result;
}
