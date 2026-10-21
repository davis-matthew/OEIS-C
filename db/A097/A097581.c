/*
 * A097581  a(n) = 3*2^floor((n-1)/2) + (-1)^n.
 * https://oeis.org/A097581
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*2^{floor((n-1)/2)} + (-1)^n, offset 1
 * Range:     u8 n <= 14, u16 n <= 30, u32 n <= 62, u64 n <= 126,
 *            u128 n <= 254, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* for n >= 1  */
    idx_t k = (n - 1) / 2;               /* exponent: floor((n-1)/2) */
    seq_t power_of_two = 1;              /* 2^k */
    for (idx_t i = 0; i < k; ++i)        /* multiply by 2, k times */
        power_of_two = power_of_two * 2;

    seq_t result = power_of_two * 3;     /* 3*2^k */

    /* add (-1)^n : 1 if n even, -1 if n odd */
    if ((n & 1u) == 0)
        result = result + 1;
    else
        result = result - 1;

    return result;
}
