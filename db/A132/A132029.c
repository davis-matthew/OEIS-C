/*
 * A132029  Product{0<=k<=floor(log_5(n)), floor(n/5^k)}, n>=1.
 * https://oeis.org/A132029
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{k=0}^{⌊log_5 n⌋} ⌊n / 5^k⌋ (defined for n ≥ 1, a(1) =
 *            1)
 * Range:     u8 n <= 36, u16 n <= 204, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* ensure n ≥ 1 */
    if (n == 0) {
        return 0; /* undefined for 0, return 0 to avoid crash */
    }

    seq_t result = 1;
    idx_t temp = n;

    while (temp > 0) {
        result *= (seq_t)temp;
        temp /= 5;
    }

    return result;
}
