/*
 * A085333  a(n) is the least n-th power of a prime that is the sum of two
 *          positive cubes, or 0 if no solution exists (for n=3k).
 * https://oeis.org/A085333
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 if n % 3 == 0; otherwise a(n) = 2^n when n % 3 == 1,
 *            and a(n) = 3^n when n % 3 == 2
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 22, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t seq_pow(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp) {
        if (exp & ((idx_t)1)) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* for safety, although the offset is 1 */
    switch (n % 3) {
        case 0: return (seq_t)0;
        case 1: return seq_pow((seq_t)2, n);
        case 2: return seq_pow((seq_t)3, n);
    }
    return (seq_t)0; /* unreachable but keeps the compiler happy */
}
