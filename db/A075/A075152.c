/*
 * A075152  Number of possible permutations of a Rubik cube of size n X n X n.
 * https://oeis.org/A075152
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 for n=1; a(2) = 7!*3⁶; a(3) = 8!*3⁷*12!*2¹⁰; for n≥4:
 *            a(n) = a(n‑2)*24⁶*(24!/24⁶)^(n‑2)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 2, u128 n <= 3,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* fast exponentiation for seq_t base and idx_t exponent */
static seq_t pow_seq(const seq_t base, idx_t exp)
{
    seq_t result = 1;
    seq_t b = base;
    while (exp != 0) {
        if (exp & 1) result *= b;
        b *= b;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    /* lazy static initialization of constants */
    static seq_t fac24 = 0, pow24 = 0, r = 0, a2 = 0, a3 = 0;
    static int init = 0;
    if (!init) {
        init = 1;
        /* 24! */
        fac24 = 1;
        for (idx_t i = 2; i <= 24; ++i) fac24 *= i;
        /* 24⁶ */
        pow24 = 1;
        for (idx_t i = 0; i < 6; ++i) pow24 *= 24;
        /* r = 24!/24⁶ */
        r = fac24 / pow24;

        /* a(2) = 7! * 3⁶ */
        seq_t f7 = 1;
        for (idx_t i = 2; i <= 7; ++i) f7 *= i;
        seq_t p3_6 = 1;
        for (idx_t i = 0; i < 6; ++i) p3_6 *= 3;
        a2 = f7 * p3_6;

        /* a(3) = 8! * 3⁷ * 12! * 2¹⁰ */
        seq_t f8 = 1;
        for (idx_t i = 2; i <= 8; ++i) f8 *= i;
        seq_t p3_7 = 1;
        for (idx_t i = 0; i < 7; ++i) p3_7 *= 3;
        seq_t f12 = 1;
        for (idx_t i = 2; i <= 12; ++i) f12 *= i;
        seq_t p2_10 = 1;
        for (idx_t i = 0; i < 10; ++i) p2_10 *= 2;
        a3 = f8 * p3_7 * f12 * p2_10;
    }

    if (n == 2) return a2;
    if (n == 3) return a3;

    if ((n & 1) == 0) {            /* even n */
        idx_t k = (n - 2) / 2;      /* # of 2‑step increments from 2 to n */
        seq_t res = a2;
        res *= pow_seq(pow24, k);
        res *= pow_seq(r, k * (k + 1));
        return res;
    } else {                       /* odd n */
        idx_t k = (n - 3) / 2;      /* # of 2‑step increments from 3 to n */
        seq_t res = a3;
        res *= pow_seq(pow24, k);
        res *= pow_seq(r, k * (k + 2));
        return res;
    }
}
