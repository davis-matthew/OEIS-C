/*
 * A134654  Duplicate of A038521.
 * https://oeis.org/A134654
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(0) = 0 , a(1) = 0. For n ≥ 2: n = 4m → a(n) = 2^(4m‑2) n =
 *            4m+1 → a(n) = 2^(2m‑1) · (2^(2m) – (‑1)^m) n = 4m+2 → a(n) =
 *            2^(4m) + (‑1)^m · 2^(2m) n = 4m+3 → a(n) = 2^(2m) · (2^(2m+1) –
 *            (‑1)^m)
 * Range:     i8 n <= 9, i16 n <= 17, i32 n <= 24, i64 n <= 24, i128 n <= 24,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

/* Return 2^exp as seq_t.  For seq_t types larger than 64 bits the utility
   function implements an arbitrary‑precision power; when seq_t is a
   64‑bit integer we use a shift for exp < 63 to avoid undefined behaviour. */
static inline seq_t two_power(idx_t exp) {
    if (exp < 63) {
        return (seq_t)1 << exp;
    } else {
        return pow2(exp);
    }
}

seq_t a(idx_t n) {
    if (n == 0 || n == 1) return 0;

    idx_t r = n & 3;          /* n mod 4 */
    seq_t res = 0;

    switch (r) {
        case 0:   /* n = 4m */
        {
            idx_t m = n / 4;
            idx_t exp = 4 * m - 2;
            res = two_power(exp);
            break;
        }

        case 1:   /* n = 4m + 1 */
        {
            idx_t m = (n - 1) / 4;
            /* m ≥ 1 here, so exponent ≥ 1 */
            seq_t pow2_2m   = pow2(2 * m);
            idx_t  exp      = 2 * m - 1;
            seq_t pow2_exp  = two_power(exp);
            seq_t sign      = (m & 1) ? (seq_t)(-1) : (seq_t)1;
            seq_t term2     = pow2_2m - sign;
            res = pow2_exp * term2;
            break;
        }

        case 2:   /* n = 4m + 2 */
        {
            idx_t m = (n - 2) / 4;
            seq_t term1 = pow2(4 * m);
            seq_t term2 = ((m & 1) ? (seq_t)(-1) : (seq_t)1) * pow2(2 * m);
            res = term1 + term2;
            break;
        }

        case 3:   /* n = 4m + 3 */
        {
            idx_t m = (n - 3) / 4;
            seq_t pow2_2m   = pow2(2 * m);
            seq_t pow2_2m1  = pow2(2 * m + 1);
            seq_t term2     = pow2_2m1 - ((m & 1) ? (seq_t)(-1) : (seq_t)1);
            res = pow2_2m * term2;
            break;
        }
    }
    return res;
}
