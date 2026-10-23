/*
 * A016367  259th cyclotomic polynomial.
 * https://oeis.org/A016367
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   not stated
 * Range:     i8 n <= 216, i16 n <= 216, i32 n <= 216, i64 n <= 216,
 *            i128 n <= 216, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = coefficient of x^n in the 259th cyclotomic polynomial Φ_259(x)
 *
 * The algorithm builds Φ_259(x) as
 * Φ_259(x) = (x^259-1)(x-1) / (x^37-1)(x^7-1)
 * using only integer arithmetic on the coefficients.  The result is
 * memoised in a static array of length 217 (φ(259) = 216 + 1), so that
 * subsequent calls are O(1).  No dynamic allocation is used, thus the
 * implementation is safe under both C11 and C++17 builds.
 */

#include <stdbool.h>   /* for bool */

seq_t a(idx_t n)
{
    /* static memoisation of the 217 coefficients */
    static bool init = false;
    static seq_t coeffs[217];
    if (!init) {
        /* working buffers; largest required length is 261 */
        enum { MAXW = 262 };
        static seq_t poly[MAXW], out[MAXW];

        idx_t cur_len = 1;
        for (idx_t i = 0; i < MAXW; ++i) poly[i] = 0;
        poly[0] = 1;   /* start with the constant 1 */

        /* --- multiply by (x^259 - 1) --------------------------------- */
        idx_t d = 259;
        idx_t new_len = cur_len + d;                /* 260 */
        for (idx_t i = 0; i < new_len; ++i) out[i] = 0;
        for (idx_t i = 0; i < cur_len; ++i) {
            out[i]     += poly[i];
            out[i + d] -= poly[i];
        }
        for (idx_t i = 0; i < new_len; ++i) poly[i] = out[i];
        cur_len = new_len;

        /* --- multiply by (x - 1) ------------------------------------- */
        d = 1;
        new_len = cur_len + d;                      /* 261 */
        for (idx_t i = 0; i < new_len; ++i) out[i] = 0;
        for (idx_t i = 0; i < cur_len; ++i) {
            out[i]     += poly[i];
            out[i + 1] -= poly[i];
        }
        for (idx_t i = 0; i < new_len; ++i) poly[i] = out[i];
        cur_len = new_len;

        /* --- divide by (x^37 - 1) ----------------------------------- */
        d = 37;
        new_len = cur_len - d;                      /* 224 */
        for (idx_t i = 0; i < new_len; ++i) out[i] = 0;
        for (idx_t i = 0; i < new_len; ++i) {
            if (i < d)
                out[i] = -poly[i];
            else
                out[i] = out[i - d] - poly[i];
        }
        for (idx_t i = 0; i < new_len; ++i) poly[i] = out[i];
        cur_len = new_len;

        /* --- divide by (x^7 - 1) ------------------------------------ */
        d = 7;
        new_len = cur_len - d;                      /* 217 */
        for (idx_t i = 0; i < new_len; ++i) out[i] = 0;
        for (idx_t i = 0; i < new_len; ++i) {
            if (i < d)
                out[i] = -poly[i];
            else
                out[i] = out[i - d] - poly[i];
        }
        for (idx_t i = 0; i < new_len; ++i) poly[i] = out[i];
        cur_len = new_len;

        /* store the coefficients */
        for (idx_t i = 0; i < cur_len; ++i) coeffs[i] = poly[i];
        init = true;
    }

    /* The polynomial is zero for n ≥ 217 (φ(259)). */
    if (n >= (idx_t)217) return (seq_t)0;
    return coeffs[n];
}
