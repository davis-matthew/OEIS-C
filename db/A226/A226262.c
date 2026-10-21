/*
 * A226262  Number of additive Z_2 Z_8 codes of a certain type (see
 *          Siap-Aydogdu for precise definition).
 * https://oeis.org/A226262
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(7*n) - 7*2^(6*n-2) + 7*2^(5*n-3) - 2^(4*n-3)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* compute 2^exp for any non‑negative exp; implemented with
   multiplication by 2 to keep it valid for all seq_t types. */
static seq_t pow2_loop(idx_t exp)
{
    seq_t r = (seq_t)1;
    while (exp)
    {
        r = r * (seq_t)2;
        --exp;
    }
    return r;
}

seq_t a(idx_t n)
{
    /* the function is defined only for n >= 1; behaviour for n < 1 is
       unspecified. */
    idx_t exp_7n   = (idx_t)7 * n;
    idx_t exp_6n_2 = (idx_t)6 * n - (idx_t)2;
    idx_t exp_5n_3 = (idx_t)5 * n - (idx_t)3;
    idx_t exp_4n_3 = (idx_t)4 * n - (idx_t)3;

    seq_t res = pow2_loop(exp_7n);
    res -= (seq_t)7 * pow2_loop(exp_6n_2);
    res += (seq_t)7 * pow2_loop(exp_5n_3);
    res -= pow2_loop(exp_4n_3);

    return res;
}
