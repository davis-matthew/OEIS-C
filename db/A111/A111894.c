/*
 * A111894  Number of numbers m <= n such that 4 equals the second digit after
 *          decimal point of square root of n in decimal representation.
 * https://oeis.org/A111894
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: For every m from 1 to n, compute the integer
 * sqrt of m·1,000,000.  The second fractional digit of √m
 * equals (floor(√(m·1,000,000))/10) % 10.  Count those for which
 * this digit is 4.  (This works because
 * floor(√m · 100) = floor(√(m·10 000)); so using 1 000 000
 * gives an extra decimal place, allowing extraction of the
 * hundredths digit by division and modulo.)
 */

static inline unsigned long long isqrt_uint128(unsigned __int128 x)
{
    unsigned __int128 low = 0;
    /* The largest possible square root fits in 64 bits. */
    unsigned __int128 high = ((unsigned __int128)1 << 64) - 1;
    while (low <= high) {
        unsigned __int128 mid = (low + high) >> 1;
        unsigned __int128 sq  = mid * mid;
        if (sq <= x) {
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    /* low is the first value such that low*low > x */
    return (unsigned long long)(low - 1);
}

seq_t a(idx_t n)
{
    seq_t count = 0;
    for (idx_t m = 1; m <= n; ++m) {
        unsigned __int128 prod = (unsigned __int128)m * 1000000ULL; /* m · 10^6 */
        unsigned long long root = isqrt_uint128(prod);
        unsigned int digit = (root / 10) % 10; /* hundreds place of √m */
        if (digit == 4) {
            ++count;
        }
    }
    return count;
}
