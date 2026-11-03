/*
 * A159863  a(3)(baseN) = least number m such that the quotient m/3 is
 *          obtained merely by shifting the leftmost digit (MSD) of m to the
 *          right end (LSD), and the multiple 3m by shifting the LSD of m to
 *          the MSD, in order of increasing number base, N.
 * https://oeis.org/A159863
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  base, nonn, fini, full
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 4, u32 n <= 5, u64 n <= 9, u128 n <= 11,
 *            big any n
 * Validated: full, pass, 8 terms through n = 11 (2026-11-02)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A159863 (OEIS)
 *
 *  a(n) is the decimal integer obtained by writing the least base-n number
 *  m that satisfies
 *
 *     m / 3   = rotate_left(m)   (MSD moved to LSD)
 *     3 * m   = rotate_right(m)  (LSD moved to MSD)
 *
 *  The function returns the decimal representation of the base‑n digits of
 *  that number.  For n < 4 it returns 0.  The algorithm searches the
 *  digit length k, computes the base‑N power pow = N^(k-1), and tests
 *  every possible most significant digit a (1 … N-1) for the
 *  Diophantine relation
 *
 *          a * (pow - 3)  is divisible by 3*N - 1
 *          t = a * (pow - 3) / (3*N - 1)  < pow
 *
 *  where t is the remaining lower k-1 digits.  The two shift
 *  conditions are then verified explicitly.  The first such m found
 *  (i.e. the one with the smallest k) is returned.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

#include "../utils/gcd.h"

static seq_t digits_as_decimal(idx_t base, seq_t ms, seq_t low, idx_t k)
{
    /* ms : most significant digit (a)  */
    /* low : lower k-1 digits as a number in base 'base' */
    /* k  : total number of base‑digits */

    seq_t result = ms;                 /* start with most significant digit */

    if (k == 1)                        /* nothing else to append */
        return result;

    size_t len = (size_t)(k - 1);
    unsigned short *digits = (unsigned short *)malloc(len * sizeof(unsigned short));
    if (!digits)          /* unlikely */
        return (seq_t)0;

    /* Extract digits of 'low' from least to most significant */
    seq_t tmp = low;
    for (size_t i = 0; i < len; ++i) {
        digits[len - 1 - i] = (unsigned short)(tmp % (seq_t)base);
        tmp /= (seq_t)base;
    }

    /* Build the decimal result from the digits */
    for (size_t i = 0; i < len; ++i) {
        result = result * (seq_t)10 + (seq_t)digits[i];
    }

    free(digits);
    return result;
}

static seq_t minimal_m_decimal(idx_t N)
{
    if (N < 4)                 /* offset: a(n)=0 for n<4 */
        return (seq_t)0;

    seq_t pow = 1;                 /* pow = N^(k-1), start with k=1 */
    seq_t denom = (seq_t)(3ULL * N - 1ULL);   /* constant for this base */

    for (idx_t k = 1; ; ++k) {
        /* Skip too small pow that would make (pow-3) negative */
        if (pow < (seq_t)3) {
            pow *= (seq_t)N;          /* advance to next digit length */
            continue;
        }

        seq_t pow_minus3 = pow - (seq_t)3;
        seq_t pow_div_N  = pow / (seq_t)N;   /* integer division, N>1 */

        /* Try every possible most significant digit a */
        for (idx_t a = 1; a < N; ++a) {
            seq_t a_seq   = (seq_t)a;
            seq_t lhs     = a_seq * pow_minus3;           /* a*(pow-3) */

            /* Must be divisible by denom */
            if (lhs % denom != 0)
                continue;

            seq_t t = lhs / denom;
            if (t >= pow)       /* t must fit in k-1 digits */
                continue;

            seq_t m = a_seq * pow + t;
            if (m % (seq_t)3 != 0)   /* safety check */
                continue;

            /* Verify right‑shift condition */
            seq_t c = t % (seq_t)N;
            seq_t B = t / (seq_t)N;
            seq_t right = c * pow + a_seq * pow_div_N + B;

            if (m * (seq_t)3 != right)
                continue;

            /* All conditions satisfied – return decimal digits */
            return digits_as_decimal(N, a_seq, t, k);
        }

        pow *= (seq_t)N;          /* increase digit length */
    }
}

/* External function required by the build */
seq_t a(idx_t n)
{
    return minimal_m_decimal(n);
}
