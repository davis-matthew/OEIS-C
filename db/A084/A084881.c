/*
 * A084881  Number of (k,m,n)-multiantichains of multisets with k=3 and m=4.
 * https://oeis.org/A084881
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A084881 (k=3, m=4)
   Formula: a(n) = (81^n - 12*54^n + 24*42^n + 4*36^n - 24*31^n
                     + 18*27^n + 6*26^n - 108*18^n + 108*14^n
                     + 83*9^n - 166*6^n + 90*3^n) / 24
   Computed by fast exponentiation (square‑and‑multiply).  */

#include <stdint.h>

/* Fast exponentiation: base^exp, where base is a small integer cast to seq_t. */
static seq_t pow_seq(idx_t exp, seq_t base)
{
    seq_t result = (seq_t)1;
    seq_t b = base;
    while (exp > 0)
    {
        if (exp & (idx_t)1)
            result *= b;
        b *= b;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* powers of the base constants */
    seq_t p81  = pow_seq(n, (seq_t)81);
    seq_t p54  = pow_seq(n, (seq_t)54);
    seq_t p42  = pow_seq(n, (seq_t)42);
    seq_t p36  = pow_seq(n, (seq_t)36);
    seq_t p31  = pow_seq(n, (seq_t)31);
    seq_t p27  = pow_seq(n, (seq_t)27);
    seq_t p26  = pow_seq(n, (seq_t)26);
    seq_t p18  = pow_seq(n, (seq_t)18);
    seq_t p14  = pow_seq(n, (seq_t)14);
    seq_t p9   = pow_seq(n, (seq_t)9);
    seq_t p6   = pow_seq(n, (seq_t)6);
    seq_t p3   = pow_seq(n, (seq_t)3);

    /* aggregate the terms according to the formula */
    seq_t num = p81;
    num += (seq_t)24 * p42;
    num += (seq_t)4  * p36;
    num += (seq_t)18 * p27;
    num += (seq_t)6  * p26;
    num += (seq_t)108 * p14;
    num += (seq_t)83  * p9;
    num += (seq_t)90  * p3;

    num -= (seq_t)12  * p54;
    num -= (seq_t)24  * p31;
    num -= (seq_t)108 * p18;
    num -= (seq_t)166 * p6;

    /* divide by 24; the division is exact */
    return num / (seq_t)24;
}
