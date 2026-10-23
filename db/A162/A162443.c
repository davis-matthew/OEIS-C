/*
 * A162443  Numerators of the BG1[ -5,n] coefficients of the BG1 matrix.
 * https://oeis.org/A162443
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, frac, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 11, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   gcd_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A162443 – numerators of BG1[−5,n] coefficients.
 * Formula: a(n) = numer(((1-8*n+12*n^2)*4^(n-1)*(n-1)!^2)/(2*n-2)!)
 * where  n ≥ 1 (offset 1).  The fraction is reduced to lowest terms
 * before extracting the numerator.
 *
 * Implementation uses only integer arithmetic on `seq_t` (which may be a
 * built‑in unsigned type or an arbitrary‑precision integer class).
 * Functions required from the build system are not used directly; all
 * necessary calculation is performed with standard operators.  The
 * algorithm is O(n) per call because it builds the binomial coefficient
 * C(2n‑2,n‑1) in O(n) multiplications and uses binary exponentiation for
 * 4^(n‑1).  An Euclidean algorithm reduces the fraction.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd_seq.h"

/* Helper: compute binomial coefficient C(n,k) as seq_t.
 * Works for all supported seq_t types.  Uses the multiplicative formula.
 */
static seq_t binomial(idx_t n, idx_t k)
{
    if (k > n - k) {
        k = n - k;            /* use the smaller argument             */
    }
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i) / (seq_t)i;
    }
    return res;
}

/* Helper: Euclidean GCD for seq_t */

/* Main sequence function */
seq_t a(idx_t n)
{
    /* --- 1. Polynomial part: 12*n^2 - 8*n + 1  --- */
    seq_t poly = (seq_t)12 * (seq_t)n * (seq_t)n
               - (seq_t)8 * (seq_t)n
               + (seq_t)1;

    /* --- 2. 4^(n-1) (binary exponentiation) --- */
    idx_t exp = n - 1;
    seq_t pow4 = 1;
    seq_t base = 4;
    while (exp > 0) {
        if (exp & 1u) pow4 *= base;
        base *= base;
        exp >>= 1u;
    }

    /* --- 3. Numerator before reduction --- */
    seq_t num = poly * pow4;

    /* --- 4. Denominator: binomial coefficient C(2*n-2, n-1) --- */
    idx_t two_n_minus_2 = 2 * n - 2;
    idx_t n_minus_1     = n - 1;
    seq_t denom = binomial(two_n_minus_2, n_minus_1);

    /* --- 5. Reduce fraction to lowest terms --- */
    seq_t g = gcd_seq(num, denom);
    return num / g;
}
