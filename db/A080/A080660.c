/*
 * A080660  Order of group of n X n X n Rubik cube, under assumptions s, m,
 *          not-i.
 * https://oeis.org/A080660
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1 a(2) = 7! * 3^6 a(3) = 8! * 3^7 * 12! * 2^10 * 4^6 / 2
 *            a(n) = a(n-2) * 24! * (24!/2)^(n-3) for n > 3 (i.e. for each
 *            increase of n by 2 the multiplicative factor is F * G^(n-3) with
 *            F = 24! and G = 24!/2
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 2, u128 n <= 3,
 *            big any n
 * Helpers:   factorial_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factorial_of.h"

static seq_t pow_seq_t(seq_t base, idx_t exp) {
    seq_t res = 1;
    for (idx_t i = 0; i < exp; ++i)
        res *= base;
    return res;
}

seq_t a(idx_t n)
{
    /* Base case n = 1 */
    if (n < 2)
        return 1;

    /* Pre‑compute the small factorial values when first needed */
    static seq_t base_even = 0; /* a(2) */
    static seq_t base_odd  = 0; /* a(3) */

    if (!base_even) {
        seq_t f7   = factorial_of(7);
        seq_t f12  = factorial_of(12);
        seq_t f8   = factorial_of(8);
        seq_t pow3_6 = pow_seq_t(3, 6);
        seq_t pow3_7 = pow_seq_t(3, 7);
        seq_t pow2_10 = pow_seq_t(2, 10);
        seq_t pow4_6  = pow_seq_t(4, 6);

        base_even = f7 * pow3_6;

        seq_t temp = f12 * pow2_10 * pow4_6;
        temp /= 2;                       /* division by 2 – exact */
        base_odd  = f8 * pow3_7 * temp;
    }

    if (n == 2)
        return base_even;
    if (n == 3)
        return base_odd;

    /* Prepare the constant factors that appear in the recurrence */
    static seq_t F   = 0; /* 24! */
    static seq_t G   = 0; /* 24! / 2 */
    static seq_t G2  = 0; /* G^2 */
    if (!F) {
        F  = factorial_of(24);
        G  = F / 2;
        G2 = G * G;
    }

    /* Select which branch (even or odd) contains the desired term */
    seq_t value;
    idx_t start_idx;     /* first n ≥ 4 in this parity that we must calculate */
    seq_t g_power;       /* G^(current_idx-3); starts with G or G^2 */

    if (n % 2 == 0) {            /* even n */
        value = base_even;
        start_idx = 4;
        g_power  = G;           /* for n = 4 exponent is 1 */
    } else {                     /* odd n */
        value = base_odd;
        start_idx = 5;
        g_power  = G * G;       /* for n = 5 exponent is 2 */
    }

    for (idx_t i = start_idx; i <= n; i += 2) {
        seq_t factor = F * g_power;   /* F * G^(i-3) */
        value *= factor;
        g_power *= G2;                /* next exponent increases by 2 */
    }

    return value;
}
