/*
 * A129873  Sequence s_n arising in enumeration of arrays of directed blocks
 *          (see Quaintance reference for precise definition).
 * https://oeis.org/A129873
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) satisfies the cubic equation
 *   (8*t^4 - t^2)*S(t)^3 + (24*t^3 - 2*t^2 - 4*t)*S(t)^2
 *   + (24*t^2 + 2*t - 3)*S(t) + 8*t + 3 = 0,
 *   where S(t) = ∑_{k≥0} a(k) t^k and a(0)=1.  
 * Sequence a(n) for n ≥ 1 is the coefficient of t^n in S(t).  
 */

#include <stdlib.h>
#include <stdint.h>

/* The external integer types */
extern seq_t a(idx_t n);

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static idx_t  computed = 0;          /* largest index already computed  */
    static idx_t  capacity = 0;          /* size of the allocated arrays    */
    static seq_t *s  = NULL;            /* a(k)  (k ≥ 0)  */
    static seq_t *c2 = NULL;            /* coefficient of t^k in S^2      */
    static seq_t *c3 = NULL;            /* coefficient of t^k in S^3      */

    /* Guard against n==0 – not part of the sequence, but return 1 for consistency */
    if (n == 0) return (seq_t)1;

    /* Ensure arrays large enough to hold index n */
    if ((size_t)n + 1 > capacity) {
        size_t new_cap = (size_t)n + 1;
        s  = (seq_t *)realloc(s ,  new_cap * sizeof(seq_t));
        c2 = (seq_t *)realloc(c2, new_cap * sizeof(seq_t));
        c3 = (seq_t *)realloc(c3, new_cap * sizeof(seq_t));

        /* If this is the first allocation, initialise the constant term */
        if (capacity == 0) {
            s[0]  = (seq_t)1;     /* a(0) = 1 from the equation */
            c2[0] = (seq_t)1;     /* (S)^2 coeff at t^0 = a(0)^2 */
            c3[0] = (seq_t)1;     /* (S)^3 coeff at t^0 = a(0)^3 */
        }
        capacity = new_cap;
    }

    /* Compute terms up to n (if not already computed) */
    for (idx_t k = computed + 1; k <= n; ++k) {
        seq_t tmp = (seq_t)0;

        /* contributions from the polynomial equation */
        if (k >= 4) tmp += (seq_t)8 * c3[k - 4];
        if (k >= 2) tmp -=                                 c3[k - 2];
        if (k >= 3) tmp += (seq_t)24 * c2[k - 3];
        if (k >= 2) tmp -= (seq_t)2  * c2[k - 2];
        if (k >= 1) tmp -= (seq_t)4  * c2[k - 1];
        if (k >= 2) tmp += (seq_t)24 * s[k - 2];
        if (k >= 1) tmp += (seq_t)2  * s[k - 1];
        if (k == 1) tmp += (seq_t)8;    /* 8*t  term for n==1 */

        /* a(k) = coefficient at t^k in S(t) */
        s[k] = tmp / (seq_t)3;          /* division exact by construction */

        /* Update convolution tables needed for future terms */

        /* Compute coefficient of t^k in S^2 :  C2(k) = Σ_{i=0}^k s[i]*s[k-i] */
        seq_t sum_c2 = (seq_t)0;
        for (idx_t i = 0; i <= k; ++i)
            sum_c2 += s[i] * s[k - i];
        c2[k] = sum_c2;

        /* Compute coefficient of t^k in S^3 :  C3(k) = Σ_{i=0}^k s[i]*C2(k-i) */
        seq_t sum_c3 = (seq_t)0;
        for (idx_t i = 0; i <= k; ++i)
            sum_c3 += s[i] * c2[k - i];
        c3[k] = sum_c3;
    }

    computed = n;
    return s[n];
}
