/*
 * A120813  Number of permutations of length n with exactly 5 occurrences of
 *          the pattern 2-13.
 * https://oeis.org/A120813
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((n+4)(-108 - 192 n + 3 n^2 + 8 n^3 + n^4)) / (120 (n+7))
 *            * binomial[2n, n-6]
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 14, u64 n <= 28, u128 n <= 58,
 *            big any n
 * Helpers:   binom, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"
#include "../utils/gcd.h"

/* Multiply two seq_t values while avoiding overflow on integer types.
   For C++ arbitrary‑precision types the normal multiplication is fine. */
#ifdef __cplusplus
inline seq_t mul_seq(seq_t a, seq_t b) { return a * b; }
#else
static inline seq_t mul_seq(seq_t a, seq_t b) { return (seq_t)((unsigned __int128)a * (unsigned __int128)b); }
#endif

seq_t a(idx_t n)
{
    /* For n < 6 binomial[2n, n-6] is zero, hence a(n)=0. */
    if (n < 6) return 0;

    seq_t N = (seq_t)n;

    /* Compute the polynomial part: n^4 + 8 n^3 + 3 n^2 - 192 n - 108 */
    seq_t n2 = mul_seq(N, N);          /* n^2  */
    seq_t n3 = mul_seq(n2, N);         /* n^3  */
    seq_t n4 = mul_seq(n3, N);         /* n^4  */

    seq_t poly = n4;
    poly = poly + mul_seq(n3, (seq_t)8);
    poly = poly + mul_seq(n2, (seq_t)3);
    poly = poly - mul_seq(N   , (seq_t)192);
    poly = poly - (seq_t)108;

    /* Numerator: (N+4) * poly */
    seq_t num = mul_seq(N + (seq_t)4, poly);

    /* Denominator: 120 * (N+7) */
    seq_t denom = mul_seq((seq_t)120, N + (seq_t)7);

    /* Binomial coefficient binom(2N, N-6)   (valid because n>=6) */
    seq_t bin = binom((idx_t)(2 * n), (idx_t)(n - 6));

    /* Reduce common factors first to keep intermediate values small. */
    seq_t g = gcd(bin, denom);
    if (g > 1) { bin /= g; denom /= g; }

    g = gcd(num, denom);
    if (g > 1) { num /= g; denom /= g; }

    /* Final value: (num * bin) / denom */
    seq_t prod = mul_seq(num, bin);
    seq_t result = prod / denom;   /* division is exact by the formula */

    return result;
}
