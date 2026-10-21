/*
 * A074005  Number of elements of GF(3^n) with trace 1 and subtrace 2.
 * https://oeis.org/A074005
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 3^(n-2) + s(n)·3^⌊(n-2)/2⌋ where s(n) = 0 if n ≡ 0 (mod
 *            3) s(n) = (-1)^{⌊(n/3)/2⌋+1} if n ≡ 1 (mod 3) s(n) =
 *            (-1)^{⌊(n/3)/2⌋} if n ≡ 2 (mod 3) (The exponent ⌊(n-2)/2⌋ is the
 *            integer floor.)
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 21, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1.  Handle the single special
       case before doing any exponentiations to avoid underflow. */
    if (n == 1)
        return 0;

    seq_t three = (seq_t)3;

    /* 3^(n-2) is needed for all non‑trivial cases. */
    seq_t pow_base = big_pow(three, (seq_t)(n - 2));

    /* When n is a multiple of 3 the correction term is zero. */
    if ((n % 3) == 0)
        return pow_base;

    /* Exponent for the correction term: ⌊(n-2)/2⌋ .
       This is an integer division in idx_t (unsigned). */
    idx_t small_exp = (n - 2) / 2;
    seq_t pow_small = big_pow(three, (seq_t)small_exp);

    /* k = ⌊n/3⌋  (integer division). */
    idx_t k = n / 3;
    /* t = ⌊k/2⌋  – the parity that controls the sign. */
    idx_t t = k / 2;
    int sign;

    if ((n % 3) == 1) {               /* n ≡ 1 (mod 3) */
        /* sign = +1 when t is odd,  -1 when t is even. */
        sign = (t & 1) ? +1 : -1;
    } else {                           /* n ≡ 2 (mod 3) */
        /* sign = +1 when t is even,  -1 when t is odd. */
        sign = (t & 1) ? -1 : +1;
    }

    if (sign > 0)
        return pow_base + pow_small;
    else
        return pow_base - pow_small;
}
