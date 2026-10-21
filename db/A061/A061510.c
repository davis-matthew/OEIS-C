/*
 * A061510  Write n in decimal, omit 0's, raise each digit k to k-th power and
 *          multiply.
 * https://oeis.org/A061510
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, look, nonn
 * Formula:   a(n) = ∏_{d∈digits of n, d≠0} d^d (empty product = 1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 38, u64 n <= 498,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;
    while (n > 0) {
        int digit = n % 10;
        n /= 10;
        if (digit == 0)
            continue;
        seq_t part = 1;
        for (int i = 0; i < digit; ++i)
            part *= digit;
        result *= part;
    }
    return result;
}
