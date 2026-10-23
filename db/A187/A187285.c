/*
 * A187285  Smallest multiple of n beginning with 1.
 * https://oeis.org/A187285
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) is the smallest k*n (k∈{1,2,3,4,5}) whose decimal
 *            representation begins with the digit 1
 * Range:     u8 n <= 199, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    for (idx_t k = 1; k <= 5; ++k)
    {
        seq_t prod = n_seq * (seq_t)k;
        seq_t temp = prod;
        while (temp >= 10)
            temp /= 10;
        if (temp == 1)
            return prod;
    }
    return 5 * n_seq; /* fallback, should never happen */
}
