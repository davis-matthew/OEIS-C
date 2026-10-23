/*
 * A035181  a(n) = Sum_{d|n} Kronecker(-9, d).
 * https://oeis.org/A035181
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Sum_{d|n} Kronecker(-9, d)
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Multiplicative version:
 * a(2^e) = e+1; a(3^e)=1;
 * a(p^e) = e+1 if p==1 mod 4;
 * a(p^e) = (1+(-1)^e)/2 if p==3 mod 4 and p>3
 */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)   /* undefined but return 0 for safety */
        return (seq_t)0;

    seq_t res = 1;
    idx_t tn = n;

    /* factor 2 */
    idx_t e = 0;
    while ((tn & 1ULL) == 0) {
        tn >>= 1;
        e++;
    }
    if (e) res = res * ((seq_t)e + 1);

    /* factor 3 */
    e = 0;
    while (tn % 3 == 0) {
        tn /= 3;
        e++;
    }
    /* a(3^e) = 1 : nothing to multiply */

    /* factor remaining odd primes (>3) */
    idx_t p = 5;
    while (p <= tn / p) {            /* avoid p*p overflow */
        if (tn % p == 0) {
            e = 0;
            while (tn % p == 0) {
                tn /= p;
                e++;
            }
            if (p % 4 == 1) {
                res = res * ((seq_t)e + 1);
            } else /* p % 4 == 3 */
            {
                if (e & 1)            /* odd exponent gives 0 */
                    return (seq_t)0;
            }
        }
        p += 2;
    }

    /* remaining prime factor if any */
    if (tn > 1) {
        p = tn;
        /* exponent is 1 */
        if (p == 2) {
            /* already handled, should not occur */
            res = res * ((seq_t)2);
        } else if (p == 3) {
            /* a(3) = 1, nothing to do */
        } else if (p % 4 == 1) {
            res = res * ((seq_t)2);
        } else { /* p % 4 == 3, exponent 1 -> zero */
            return (seq_t)0;
        }
    }

    return res;
}
