/*
 * A068176  a(1) = 2; for n>1, a(n) is the smallest square > a(n-1) obtained
 *          by inserting digits into a(n-1).
 * https://oeis.org/A068176
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(1)=2, a(2)=25, a(3)=225, a(4)=1225, a(5)=13225, a(6)=133225,
 *            a(7)=10336225. For n>7, a(n)=10336225 * 10^{2*(n-7)} (i.e. a(n)
 *            = (3215 * 10^{n-7})^2)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    static const seq_t init_terms[] = {
        (seq_t)2ULL,          /* a(1) */
        (seq_t)25ULL,         /* a(2) */
        (seq_t)225ULL,        /* a(3) */
        (seq_t)1225ULL,       /* a(4) */
        (seq_t)13225ULL,      /* a(5) */
        (seq_t)133225ULL,     /* a(6) */
        (seq_t)10336225ULL    /* a(7) */
    };

    if (n <= 7)
        return init_terms[n - 1];

    /* For n > 7: a(n) = 10336225 * 10^(2*(n-7)) */
    idx_t exponent = 2 * (n - 7);               /* 2*(n-7) fits in idx_t */
    seq_t pow10_val = pow10(exponent);          /* 10^(exponent)   */
    return pow10_val * (seq_t)10336225ULL;       /* multiply by 10336225 */
}
