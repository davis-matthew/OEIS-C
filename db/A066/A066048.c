/*
 * A066048  Product of smallest and greatest prime factors of n.
 * https://oeis.org/A066048
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = Pmin(n) * Pmax(n) (product of smallest and greatest prime
 *            factors)
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* n is at least 1 (offset 1).  a(1) = 1 by definition. */
    if (n == 1) return 1;

    /* Work with uint64_t for trial division.  n may be larger than 32 bits,
       but the loop condition uses i <= temp / i to avoid overflow. */
    uint64_t temp = (uint64_t)n;
    uint64_t sp = 0;          /* smallest prime factor */
    uint64_t gp = 0;          /* greatest prime factor found so far */

    for (uint64_t i = 2; i <= temp / i; ++i) {
        if (temp % i == 0) {
            /* i is a prime factor of the original n */
            if (sp == 0) sp = i;   /* first factor encountered is the smallest */
            gp = i;                /* last factor encountered in this loop is the greatest so far */

            /* Remove all occurrences of this prime factor from temp */
            while (temp % i == 0) temp /= i;
        }
    }

    /* After the loop, if temp > 1, it is a prime factor larger than sqrt(original n). */
    if (temp > 1) {
        if (sp == 0) sp = temp;       /* n itself was prime */
        if (temp > gp) gp = temp;     /* update greatest prime factor if necessary */
    }

    /* Compute the product in seq_t.  Even on very small seq_t types this
       multiplication will wrap according to the type's size as required. */
    return (seq_t)sp * (seq_t)gp;
}
