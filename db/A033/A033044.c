/*
 * A033044  Sums of distinct powers of 7.
 * https://oeis.org/A033044
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(1)=0; for n>1, a(n)=∑_{i≥0} bit_i(n-1)·7^i where bit_i(n-1) is
 *            the i‑th bit of the binary representation of n-1
 * Range:     u8 n <= 8, u16 n <= 64, u32 n <= 1024, u64 n <= 1024,
 *            u128 n <= 1024, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return 0;

    idx_t bits = n - 1;          /* n-1 has the binary digits */
    seq_t result = 0;
    seq_t pow7 = 1;              /* 7^i */

    while (bits)
    {
        if (bits & 1ULL)
            result += pow7;
        pow7 *= 7;
        bits >>= 1;
    }

    return result;
}
