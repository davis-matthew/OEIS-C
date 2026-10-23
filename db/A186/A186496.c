/*
 * A186496  Adjusted joint rank sequence of (f(i)) and (g(j)) with f(i) after
 *          g(j) when f(i)=g(j), where f(i)=5i and g(j)=j-th pentagonal
 *          number. Complement of A186495.
 * https://oeis.org/A186496
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(g(n)/5) + floor((1 + sqrt(1 + 24*g(n)))/6) -
 *            (g(n)%5==0 ? 1 : 0) where g(n) = n*(3*n - 1)/2 (the n-th
 *            pentagonal number). This gives the rank of the n-th pentagonal
 *            number in the merged sequence of multiples of 5 and pentagonal
 *            numbers, with ties resolved by placing the pentagonal nu...
 * Range:     u8 n <= 13, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   _floor_isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/_floor_isqrt_u64.h"

seq_t a(idx_t n) {
    if (n == 0)
        return 0;  /* no term for zero index; offset is 1 */

    /* Compute the n-th pentagonal number g(n) = n*(3*n - 1)/2 */
    seq_t g = (seq_t)n * (((seq_t)3 * (seq_t)n) - (seq_t)1) / (seq_t)2;

    /* Count of multiples of 5 not exceeding g: floor(g/5) */
    seq_t fcnt = g / (seq_t)5;

    /* Count of pentagonal numbers not exceeding g:
       Solve j(3j-1)/2 <= g  ->  j = floor((1 + sqrt(1+24*g))/6) */
    idx_t sqrt_arg = 1 + (uint64_t)24 * (uint64_t)g;      /* 1 + 24*g  (fits in 64-bit for allowed n) */
    idx_t sqrt_val = _floor_isqrt_u64(sqrt_arg);          /* floor(sqrt(1+24*g)) */
    seq_t gcnt = (seq_t)(( (seq_t)1 + (seq_t)sqrt_val ) / (seq_t)6);

    /* If g is a multiple of 5, reduce count by 1 because the pentagonal
       number itself is not counted in the union count below it. */
    seq_t delta = (g % (seq_t)5 == 0) ? (seq_t)1 : (seq_t)0;

    return fcnt + gcnt - delta;
}
