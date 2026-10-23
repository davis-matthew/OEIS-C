/*
 * A159774  Least number m, written in base n, such that m/2 is obtained
 *          merely by shifting the leftmost digit of m to the right end, and
 *          2m by shifting the rightmost digit of m to the left end, digits
 *          defined in base n.
 * https://oeis.org/A159774
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  base, nonn, fini, full
 * Formula:   * a(n) = least positive integer m written in base n such that *
 *            m/2 equals the left‑rotation of the base‑n digits of m * and 2·m
 *            equals the right‑rotation of the base‑n digits of m. * The
 *            algorithm searches for the minimal length L of the digit string.
 *            * For each L it tests the minimal possible l...
 * Range:     u8 none, u16 n <= 4, u32 n <= 6, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

/* Integer gcd for idx_t (unsigned 64‑bit). */
static inline idx_t int_gcd(idx_t a, idx_t b)
{
    while (b != 0)
    {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* The sequence is defined only for n ≥ 3.  For smaller n we return 0. */
    if (n < 3)
        return 0;

    /* Denominator (2n‑1) is small enough for idx_t. */
    idx_t denom_val = 2 * n - 1;
    seq_t denom = (seq_t)denom_val;

    /* P = n^(L‑1).  Initially L = 2 so that P = n. */
    seq_t p = (seq_t)n;
    idx_t p_mod = n % denom_val;      /* P modulo denom */
    idx_t L = 2;                     /* Current length of the digit string */

    for (;; ++L)
    {
        /* Compute (P – 2) mod denom. */
        idx_t a_small = (p_mod >= 2) ? (p_mod - 2) : (p_mod + denom_val - 2);

        /* Small gcd – no big‑integer arithmetic needed. */
        idx_t g_small = int_gcd(a_small, denom_val);
        idx_t unit     = denom_val / g_small;   /* Smallest d0 that works */

        if (unit <= n - 1)
        {
            /* Pre‑compute P / n for the right‑rotation test. */
            seq_t p_div_n = p / (seq_t)n;

            /* Try all admissible leading digits: multiples of 'unit'. */
            for (idx_t mult = 1; mult * unit <= n - 1; ++mult)
            {
                idx_t d0 = mult * unit;
                seq_t d0s = (seq_t)d0;

                /* r = d0 * (P – 2) / (2n – 1)   (integer division) */
                seq_t p_minus_2 = p - (seq_t)2;
                seq_t numerator  = p_minus_2 * d0s;
                seq_t r          = numerator / denom;

                /* Right‑rotation condition check. */
                seq_t t  = r % (seq_t)n;
                seq_t s  = (r - t) / (seq_t)n;
                seq_t left  = t * p + d0s * p_div_n + s;
                seq_t two_m = (d0s * p + r) * (seq_t)2;
                if (left == two_m)
                {
                    /* Success: build the decimal representation of m. */
                    seq_t m   = d0s * p + r;
                    idx_t len = L;

                    /* Allocate space for the digits. */
                    idx_t *digits = (idx_t *)malloc(len * sizeof(idx_t));
                    if (!digits)
                        return 0;  /* Allocation failure – unlikely */

                    /* Most significant digit. */
                    digits[0] = d0;

                    /* Remaining L‑1 digits (least significant to most). */
                    seq_t temp = r;
                    for (idx_t i = 1; i < len; ++i)
                    {
                        seq_t dig_seq = temp % (seq_t)n;
                        digits[len - i] = (idx_t)dig_seq;
                        temp /= (seq_t)n;
                    }

                    /* Convert to the decimal integer that represents the
                     * digit string.  All digits are < 10 for the tested
                     * values, so this works for the intended sequence. */
                    seq_t result = 0;
                    for (idx_t i = 0; i < len; ++i)
                        result = result * (seq_t)10 + (seq_t)digits[i];

                    free(digits);
                    return result;
                }
            }
        }

        /* No valid d0 for this L – try longer lengths. */
        p *= (seq_t)n;                     /* P = n^L */
        p_mod = (p_mod * n) % denom_val;   /* Update modulo for the next L */
    }
}
