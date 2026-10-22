/*
 * A000792  a(n) = max{(n - i)*a(i) : i < n}; a(0) = 1.
 * https://oeis.org/A000792
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(0) = 1, a(1) = 1; for n >= 2: a(3k) = 3^k, a(3k+1) =
 *            4*3^(k-1), a(3k+2) = 2*3^k
 * Range:     u8 n <= 15, u16 n <= 30, u32 n <= 60, u64 n <= 121,
 *            u128 n <= 242, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result;
    idx_t k, i;

    if (n <= 1)
        return (seq_t)1;

    switch (n % 3) {
    case 0:
        result = (seq_t)1;
        k = n / 3;
        break;
    case 1:
        result = (seq_t)4;
        k = (n - 4) / 3;
        break;
    default:
        result = (seq_t)2;
        k = (n - 2) / 3;
        break;
    }

    for (i = 0; i < k; i++)
        result *= 3;

    return result;
}
