/*
 * A066355  A055471(n)/(product of nonzero digits of A055471(n)).
 * https://oeis.org/A066355
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A055471(n)/(product of nonzero digits of A055471(n))
 *            Where A055471(n) is the n-th positive integer that is divisible
 *            by the product of its non‑zero decimal digits (OEIS A055471)
 * Range:     u8 n <= 49, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t m = 1;
    while (1) {
        seq_t temp = m;
        seq_t prod = 1;
        while (temp > 0) {
            seq_t digit = temp % 10;
            if (digit != 0)
                prod *= digit;
            temp /= 10;
        }
        if (m % prod == 0) {
            ++count;
            if (count == n)
                return m / prod;
        }
        ++m;
    }
}
