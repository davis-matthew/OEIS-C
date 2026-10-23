/*
 * A170227  Number of reduced words of length n in Coxeter group on 26
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^40 = I.
 * https://oeis.org/A170227
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A170227:  a(0)=1, a(1)=26, a(n)=25·a(n‑1) for n≥2
 * Equivalently: a(n)=26·25^(n-1) for n≥1.  Exponentiation by squaring. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* the sequence starts with a(0)=1 */
    if (n == 0)
        return (seq_t)1;          /* a(0) */

    /* first non‑trivial term */
    if (n == 1)
        return (seq_t)26;         /* a(1) */

    /* compute 25^(n-1) using binary exponentiation */
    seq_t power = (seq_t)1;
    seq_t base  = (seq_t)25;
    idx_t  exp  = n - 1;          /* n≥2, so exp≥1 */

    while (exp > 0) {
        if (exp & 1)
            power *= base;       /* power = power * base */
        base *= base;            /* base = base * base   */
        exp >>= 1;               /* exp = exp / 2        */
    }

    /* a(n) = 26 * 25^(n-1) */
    return (seq_t)26 * power;
}
