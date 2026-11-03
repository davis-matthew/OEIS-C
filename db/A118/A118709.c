/*
 * A118709  a(n) = determinant of n X n circulant matrix whose first row is
 *          the first n cube numbers 0, 1, ..., (n-1)^3.
 * https://oeis.org/A118709
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = (-1)^(n-1)*(n-1)^2*n^(n-2)*(n^(2n) - (n^2-3n-3 +
 *            sqrt(15n^2-18n-9)i)^n - (n^2-3n-3 - sqrt(15n^2-18n-9)i)^n +
 *            (n^2-3n+3)^n)/24
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 4, i64 n <= 7, i128 n <= 11,
 *            big any n
 * Validated: partial, pass, 14 terms through n = 13 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

/* Helper: compute S_n = b^n + c^n, where
 * b+c = 2n^2-3n-3,   bc = n^2(n^2-3n+3)   (both integers)
 * Recurrence: S_k = (2n^2-3n-3) * S_{k-1} - n^2(n^2-3n+3) * S_{k-2}.
 * Uses matrix exponentiation in O(log n) multiplications. */
static seq_t compute_S_n(idx_t n)
{
    /* For n == 0 we never call this but return 2 for completeness */
    if (n == 0) return (seq_t)2;
    seq_t n_sq = (seq_t)n * (seq_t)n;
    seq_t x    = 2 * n_sq - 3 * (seq_t)n - 3;            /* b + c   */
    seq_t b   = n_sq * (n_sq - 3 * (seq_t)n + 3);       /* b * c   */

    struct Mat
    {
        seq_t a00, a01, a10, a11;
    } M = { x, -b, 1, 0 };

    /* Exponentiate M to the (n-1)-th power */
    struct Mat R = { 1, 0, 0, 1 };     /* identity matrix */
    idx_t e = n - 1;
    while (e > 0)
    {
        if (e & 1)
        {
            struct Mat T;
            T.a00 = R.a00 * M.a00 + R.a01 * M.a10;
            T.a01 = R.a00 * M.a01 + R.a01 * M.a11;
            T.a10 = R.a10 * M.a00 + R.a11 * M.a10;
            T.a11 = R.a10 * M.a01 + R.a11 * M.a11;
            R = T;
        }
        /* Square M */
        struct Mat T;
        T.a00 = M.a00 * M.a00 + M.a01 * M.a10;
        T.a01 = M.a00 * M.a01 + M.a01 * M.a11;
        T.a10 = M.a10 * M.a00 + M.a11 * M.a10;
        T.a11 = M.a10 * M.a01 + M.a11 * M.a11;
        M = T;

        e >>= 1;
    }

    /* v1 = [S1; S0] = [x; 2] */
    seq_t S1 = x;
    seq_t S0 = 2;

    /* S_n = first component of R * v1 */
    seq_t Sn = R.a00 * S1 + R.a01 * S0;
    return Sn;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;

    seq_t n_val = (seq_t)n;
    /* Compute n^n once to reuse */
    seq_t n_pow_n = ipow(n_val, (seq_t)n);

    /* n^(2n) = (n^n)^2 */
    seq_t pow_n_2n = n_pow_n * n_pow_n;

    /* n^(n-2) = n^n / n^2 */
    seq_t pow_nm2 = n_pow_n / ((seq_t)n * (seq_t)n);

    /* Compute (n^2 - 3n + 3)^n */
    seq_t base_term = (seq_t)n * (seq_t)n - 3 * (seq_t)n + 3;
    seq_t pow_term = ipow(base_term, (seq_t)n);

    /* Compute S_n = b^n + c^n */
    seq_t S_n = compute_S_n(n);

    /* Numerator inside the parentheses */
    seq_t num = pow_n_2n - S_n + pow_term;

    /* (n-1)^2 * n^(n-2) * num */
    seq_t n_minus1 = n_val - 1;
    seq_t n_minus1_sq = n_minus1 * n_minus1;
    seq_t prod = pow_nm2 * n_minus1_sq * num;

    /* Apply the sign (-1)^(n-1) */
    if ((n % 2) == 0)
        prod = -prod;

    /* Divide by 24 */
    seq_t result = prod / 24;

    return result;
}
