/*
 * A203908  Multiplicative with a(p^e) = abs(p-e).
 * https://oeis.org/A203908
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = \prod_{p^e \parallel n} |p - e| (Multiplicative function
 *            with a(p^e) = abs(p - e))
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Computes a(n) for any n >= 1.
   Uses trial‑division factorisation.  The product is initialised to 1,
   and for every prime factor p^e we multiply by |p - e|.
   If n == 1 the empty product yields 1. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;           /* undefined for 0 but avoid UB */

    seq_t prod = 1;
    idx_t temp = n;

    if (temp > 1) {
        for (idx_t p = 2; p * p <= temp; p += (p == 2 ? 1 : 2)) {
            if (temp % p == 0) {
                idx_t e = 0;
                while (temp % p == 0) {
                    temp /= p;
                    ++e;
                }
                seq_t diff = (seq_t)(p >= e ? p - e : e - p);
                prod *= diff;
                if (prod == 0)
                    return 0;
            }
        }

        /* Remaining prime factor (if any) has exponent 1 */
        if (temp > 1) {
            seq_t diff = (seq_t)(temp >= 1 ? temp - 1 : 1 - temp);
            prod *= diff;
        }
    }

    return prod;
}
