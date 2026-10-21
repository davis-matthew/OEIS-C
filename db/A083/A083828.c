/*
 * A083828  Let x(n) = 123...n, y(n) = n...321; c(n) = delete the least
 *          significant digit of y(n) and concatenate with x(n); d(n) = delete
 *          the least significant digit of x(n) and concatenate with y(n).
 *          a(n) = c(n) - d(n).
 * https://oeis.org/A083828
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, sign
 * Formula:   a(n) = c(n) - d(n) c(n) = 10^D * floor(y(n)/10) + x(n) d(n) =
 *            10^D * floor(x(n)/10) + y(n) where x(n) = concat(1..n), y(n) =
 *            concat(n..1), D = total digits of x(n)
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 5, i64 n <= 9, i128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline unsigned int digit_count_uint64(idx_t v)
{
    /* Count decimal digits of v; assumes v fits in uint64_t. */
    if (v >= 1000000000000000000ULL) return 19;
    if (v >= 100000000000000000ULL)  return 18;
    if (v >= 10000000000000000ULL)   return 17;
    if (v >= 1000000000000000ULL)    return 16;
    if (v >= 100000000000000ULL)     return 15;
    if (v >= 10000000000000ULL)      return 14;
    if (v >= 1000000000000ULL)       return 13;
    if (v >= 100000000000ULL)        return 12;
    if (v >= 10000000000ULL)         return 11;
    if (v >= 1000000000ULL)          return 10;
    if (v >= 100000000ULL)           return 9;
    if (v >= 10000000ULL)            return 8;
    if (v >= 1000000ULL)             return 7;
    if (v >= 100000ULL)              return 6;
    if (v >= 10000ULL)               return 5;
    if (v >= 1000ULL)                return 4;
    if (v >= 100ULL)                 return 3;
    if (v >= 10ULL)                  return 2;
    return 1;
}

seq_t a(idx_t n)
{
    seq_t X = 0;  /* x(n) = concat(1..n)      */
    seq_t Y = 0;  /* y(n) = concat(n..1)      */
    seq_t pow10D = 1;  /* 10^D, D = total digits of X */
    idx_t D = 0;  /* total digit count */

    /* Build X and D, and accumulate 10^D */
    for (idx_t k = 1; k <= n; ++k) {
        unsigned int d = digit_count_uint64(k);
        D += d;

        seq_t mul = 1;
        for (unsigned int i = 0; i < d; ++i)
            mul *= 10;          /* mul = 10^d */

        pow10D *= mul;          /* pow10D = 10^(previous D + d) */
        X = X * mul + k;         /* X = X*10^d + k   */
    }

    /* Build Y (concatenation from n down to 1) */
    for (idx_t k = n; k >= 1; --k) {
        unsigned int d = digit_count_uint64(k);
        seq_t mul = 1;
        for (unsigned int i = 0; i < d; ++i)
            mul *= 10;          /* mul = 10^d */
        Y = Y * mul + k;         /* Y = Y*10^d + k   */
    }

    /* Compute floor division by 10 */
    seq_t Xp = X / 10;
    seq_t Yp = Y / 10;

    seq_t diff1 = Yp - Xp;   /* Y' - X' */
    seq_t diff2 = X - Y;     /* X - Y */

    seq_t part1 = diff1 * pow10D;  /* (Y' - X') * 10^D */
    return part1 + diff2;          /* a(n) = part1 + (X - Y) */
}
