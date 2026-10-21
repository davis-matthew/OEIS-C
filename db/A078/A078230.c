/*
 * A078230  Smallest multiple of n^n that begins with n.
 * https://oeis.org/A078230
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 8, u64 n <= 15, u128 n <= 17,
 *            big any n
 * Helpers:   digits_of_uint64, pow10_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A078230: smallest multiple of n^n that begins with n
 *
 * The algorithm:
 *   1. Compute B = n^n as an arbitrary‑precision integer.
 *   2. Let L = number of digits of n.
 *   3. The logarithm of B is Lg = n * log10(n).
 *   4. Iterate k = 1,2,3,…   (k is the multiplier).
 *        logM = Lg + log10(k)
 *        f    = fractional part of logM
 *        leading = floor(10^(f + L - 1))
 *        if leading == n
 *           compute M = B * k
 *           verify that the first L digits of M are exactly n
 *              (divide by 10^(digits(M)-L) and check equality).
 *           return M.
 *   5. The loop is guaranteed to terminate for every n ≥ 1.
 *
 * All calculations that may involve very large integers use the
 * type seq_t (either an unsigned integer or a big‑integer class).
 * The algorithm uses only integer arithmetic for final output,
 * floating‑point arithmetic only for comparing the leading digits.
 */

#include <math.h>
#include <stddef.h>
#include "../utils/digits_of_uint64.h"
#include "../utils/pow10_uint.h"

/* Static helper: exponentiation by squaring for seq_t with unsigned exponent */
static seq_t pow_seq_t(seq_t base, idx_t exp) {
    seq_t result = 1;
    seq_t b = base;
    idx_t e = exp;
    while (e > 0) {
        if (e & 1) result *= b;
        e >>= 1;
        if (e) b *= b;
    }
    return result;
}

/* Main sequence function */
seq_t a(idx_t n) {
    /* n is at least 1 by specification */
    /* 1. Compute B = n^n */
    seq_t B = pow_seq_t((seq_t)n, n);

    /* 2. Compute number of digits of n */
    int L = digits_of_uint64((uint64_t)n);

    /* 3. Compute Lg = n * log10(n) as a double */
    double log10_n = log10((double)n);
    double Lg = (double)n * log10_n;

    /* 4. Search for smallest k */
    idx_t k = 1;
    while (1) {
        /* logM = log10(k) + Lg */
        double logk = log10((double)k);
        double logM = Lg + logk;

        /* fractional part */
        double f = logM - floor(logM);

        /* leading L digits */
        double exp10 = f + (double)(L - 1);          /* exponent for 10^(f + L-1) */
        double pow10d = pow(10.0, exp10);
        uint64_t leading = (uint64_t)(pow10d + 1e-9); /* floor */

        if (leading == n) {
            /* candidate found; verify with integer arithmetic */
            seq_t M = B * (seq_t)k;

            /* digits of M = floor(logM)+1 */
            int digitsM = (int)(floor(logM)) + 1;

            /* divisor = 10^(digitsM - L) */
            seq_t divisor = pow10_uint(digitsM - L);

            seq_t prefix = M / divisor;
            if (prefix == (seq_t)n) {
                return M;
            }
        }
        ++k;
    }
}
