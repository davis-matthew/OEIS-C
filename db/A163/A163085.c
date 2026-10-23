/*
 * A163085  Product of first n swinging factorials (A056040).
 * https://oeis.org/A163085
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product_{k=1}^{n} (k! / ((⌊k/2⌋)!^2)) – product of the
 *            first n swinging factorials (A056040)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 8, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    seq_t value = (seq_t)1;   /* a(0) = 1 */
    seq_t fact = (seq_t)1;    /* factorial up to current k */
    seq_t hf   = (seq_t)1;    /* (⌊k/2⌋)! */

    for (idx_t k = 1; k <= n; ++k) {
        fact = fact * (seq_t)k;               /* k! */

        if ((k & 1) == 0)                    /* k even -> increase ⌊k/2⌋! */
            hf = hf * (seq_t)(k / 2);

        /* s(k) = fact / (hf * hf)  (exact division) */
        seq_t tmp = fact / hf;
        seq_t s_k  = tmp / hf;

        value = value * s_k;                  /* accumulate product */
    }

    return value;
}
