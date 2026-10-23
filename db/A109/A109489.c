/*
 * A109489  Value of Product[k/sd(k,2),k=1..n], where sd(k,b) is the sum of
 *          the digits of k represented in base b.
 * https://oeis.org/A109489
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini
 * Formula:   a(n) = Π_{k=1..n} k / sd(k,2) (sd(k,2) = sum of binary digits of
 *            k)
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 16, u64 n <= 27, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Generic Euclidean gcd that works for integer types and for
   arbitrary‑precision integer classes that support % and == 0. */
static inline seq_t gcd_seq(seq_t a, seq_t b)
{
    while (b != (seq_t)0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* n must be >= 1; for n==0 we return 0 (undefined in OEIS). */
    if (n == (idx_t)0)
        return (seq_t)0;

    seq_t num = (seq_t)1;   /* numerator of the product fraction */
    seq_t den = (seq_t)1;   /* denominator of the product fraction */

    for (idx_t k = (idx_t)1; k <= n; ++k) {
        /* binary digit sum of k is simply its popcount */
        uint64_t bits = __builtin_popcountll((unsigned long long)k);
        seq_t sd = (seq_t)bits;          /* sd(k,2) */
        seq_t kk = (seq_t)k;             /* k itself */

        /* Reduce the factor k with the current denominator */
        seq_t g1 = gcd_seq(kk, den);
        kk /= g1;
        den /= g1;

        /* Reduce the factor sd with the current numerator */
        seq_t g2 = gcd_seq(num, sd);
        sd /= g2;
        num /= g2;

        /* Multiply the reduced factors */
        num *= kk;
        den *= sd;

        /* Final reduction to keep numbers small */
        seq_t g3 = gcd_seq(num, den);
        num /= g3;
        den /= g3;
    }

    /* If the product is an integer, den will be 1.  Otherwise
       return the truncated integer division (the usual behaviour
       for the OEIS entry where only integer terms appear). */
    return (den == (seq_t)1) ? num : num / den;
}
