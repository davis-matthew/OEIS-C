/*
 * A088401  Duplicate of A077491.
 * https://oeis.org/A088401
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = 50*n-801 for n>=18; * a(n) = 5*n-36 for 8<=n<18; * a(n) =
 *            n/2 for even n<8; * a(n) = (n+9)/2 for odd n<8
 * Range:     i8 n <= 18, i16 n <= 24, i32 n <= 24, i64 n <= 24, i128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    long long val;
    if (n >= 18)
        val = 50LL * (long long)n - 801LL;
    else if (n >= 8)
        val = 5LL * (long long)n - 36LL;
    else if ((n & 1ULL) == 0)
        val = (long long)n / 2LL;
    else
        val = ((long long)n + 9LL) / 2LL;
    return (seq_t)val;
}
