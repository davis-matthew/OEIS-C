/*
 * A117652  a(n) = floor(n*(n+2)*(n+4)*(n-6)/192).
 * https://oeis.org/A117652
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = floor(n*(n+2)*(n+4)*(n-6)/192)
 * Range:     i8 n <= 13, i16 n <= 50, i32 n <= 801, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer type large enough for intermediate arithmetic.
   Use compiler-supported __int128 if available, otherwise fall back to long long. */
#if defined(__SIZEOF_INT128__)
    typedef __int128 int128_t;
    #define INT128_CONST(expr) expr
#else
    typedef long long int128_t;
    #define INT128_CONST(expr) expr
#endif

/* Positive divisor for the floor division */
static const int128_t DENOM = INT128_CONST(192);

/* Floor division for signed 128-bit (or 64-bit) integers with positive divisor. */
static inline int128_t floor_div_int128(int128_t a, int128_t d) {
    if (a >= 0) return a / d;
    /* a < 0 */
    int128_t neg = -a;
    int128_t q = neg / d;
    if (neg % d == 0) return -q;
    return -(q + 1);
}

/* Compute the nth term of A117652 */
seq_t a(idx_t n) {
    /* Promote index to signed 128-bit for safe arithmetic */
    int128_t N = (int128_t)n;

    /* Compute the numerator: n*(n+2)*(n+4)*(n-6) */
    int128_t prod = N * (N + 2) * (N + 4) * (N - 6);

    /* Apply floor division by 192 */
    int128_t value = floor_div_int128(prod, DENOM);

    /* Cast back to seq_t (overflow is implementation‑defined but acceptable
       when the result fits in the target type). */
    return (seq_t)value;
}
