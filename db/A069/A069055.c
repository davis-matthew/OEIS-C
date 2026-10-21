/*
 * A069055  Largest n-digit number with only prime divisors 2 or 3 (i.e., of
 *          the form 2^a * 3^b).
 * https://oeis.org/A069055
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = max{ 2^a · 3^b : 10^(n‑1) ≤ 2^a · 3^b < 10^n }
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   big_pow, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>                           /* floorl, long double */
#include "../utils/big_pow.h"               /* seq_t big_pow(seq_t,seq_t) */
#include "../utils/seq_mul.h"               /* seq_t seq_mul(seq_t,seq_t) */

seq_t a(idx_t n)
{
    /* n is at least 1 (offset 1). */
    if (n == 0) return (seq_t)1;      /* harmless fallback */

    /* Logarithms of 2 and 3 to base 10.  Use long double for precision. */
    static const long double LOG10_2 = 0.3010299956639811952137388947244930L;
    static const long double LOG10_3 = 0.4771212547196624372950278862947020L;

    /* Convert to long double once. */
    long double d_n   = (long double)n;
    long double lower = d_n - 1.0L;                     /* log10 of 10^(n-1) */

    /* Upper bound for exponents a and b.  Add a small margin. */
    size_t max_a = (size_t)(d_n / LOG10_2) + 1;
    size_t max_b = (size_t)(d_n / LOG10_3) + 1;         /* used for cast only */

    /* Track the best candidate. */
    long double best_log = -1e300L;
    size_t best_a = 0, best_b = 0;

    for (size_t a = 0; a <= max_a; ++a) {
        long double log2a = a * LOG10_2;

        /* If 2^a alone already exceeds 10^n, further a will only be larger. */
        if (log2a >= d_n) break;

        /* Compute the largest b such that 2^a·3^b < 10^n. */
        long double rest = d_n - log2a;
        long double b_exact = (rest - 1e-15L) / LOG10_3;    /* epsilon for safety */
        size_t b = (size_t)floorl(b_exact);
        if (b > max_b) b = max_b;     /* safety guard */

        long double log_val = log2a + b * LOG10_3;

        /* Only consider n‑digit numbers (>=10^(n-1)). */
        if (log_val < lower - 1e-15L) continue;

        if (log_val > best_log) {
            best_log = log_val;
            best_a = a;
            best_b = b;
        }
    }

    /* Construct the best value. */
    seq_t pow2 = big_pow((seq_t)2, (seq_t)best_a);
    seq_t pow3 = big_pow((seq_t)3, (seq_t)best_b);
    return seq_mul(pow2, pow3);
}
