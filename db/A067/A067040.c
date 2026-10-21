/*
 * A067040  a(n) = n^(sum of digits of n).
 * https://oeis.org/A067040
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n^digit_sum(n) (0^0 is defined as 1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 16, u64 n <= 47, u128 n <= 187,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline idx_t digit_sum_uint64(unsigned long long x)
{
    idx_t sum = 0;
    while (x) {
        sum += x % 10;
        x /= 10;
    }
    return sum;
}

static inline seq_t pow_seq_t(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp > 0) {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;       /* 0^0 is defined as 1 */

    idx_t sum = digit_sum_uint64((unsigned long long)n);
    seq_t base = (seq_t)n;
    return pow_seq_t(base, sum);
}
