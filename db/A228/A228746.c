/*
 * A228746  Expansion of 8 * phi(q)^4 - 7 * phi(-q)^4 in powers of q where
 *          phi() is a Ramanujan theta function.
 * https://oeis.org/A228746
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 120 * b(n) for n ≥ 1, where b is multiplicative: - b(2^e)
 *            = 1/5 (any e ≥ 1) - for an odd prime p: b(p^e) =
 *            (p+1)·b(p^(e-1)) – p·b(p^(e-2)), with b(1)=1 and b(p)=p+1. The
 *            value for n=0 is defined separately: a(0) = 1. Thus a(n) = 120·I
 *            if n is odd, a(n) = 24·I if n is even (factor 2 present...
 * Range:     u8 n <= 2, u16 n <= 314, u32 n <= 2500, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)1;

    /* Count the power of 2 in n */
    idx_t m = n;
    int e2 = 0;
    while ((m & (idx_t)1) == 0) {
        e2++;
        m >>= 1;
    }

    /* Compute product of b(p^e) over odd prime powers */
    seq_t I = (seq_t)1;
    idx_t p = 3;
    while (p * p <= m) {
        if (m % p == 0) {
            int exp = 0;
            while (m % p == 0) {
                m /= p;
                exp++;
            }
            /* Compute b(p^exp) for odd prime p */
            seq_t bp;
            if (exp == 0) {
                bp = (seq_t)1;
            } else {
                seq_t bp_prev_prev = (seq_t)1;          /* b(p^0) */
                seq_t bp_prev = (seq_t)(p + (idx_t)1);   /* b(p^1) = p+1 */
                if (exp == 1) {
                    bp = bp_prev;
                } else {
                    seq_t curr;
                    int i;
                    for (i = 2; i <= exp; i++) {
                        /* (p+1) * bp_prev - p * bp_prev_prev */
                        curr = (seq_t)(p + (idx_t)1) * bp_prev -
                               (seq_t)p * bp_prev_prev;
                        bp_prev_prev = bp_prev;
                        bp_prev = curr;
                    }
                    bp = bp_prev;
                }
            }
            I *= bp;
        }
        p += 2;
    }
    if (m > 1) { /* m is a prime > sqrt(original n) */
        I *= (seq_t)(m + (idx_t)1);
    }

    /* Multiply by the appropriate global factor */
    seq_t result;
    if (e2 > 0)
        result = (seq_t)24 * I;   /* 120 * (1/5) = 24 */
    else
        result = (seq_t)120 * I;

    return result;
}
