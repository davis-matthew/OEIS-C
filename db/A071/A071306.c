/*
 * A071306  a(n) = (1/2) * (number of n X n 0..6 matrices M with MM' mod 7 =
 *          I, where M' is the transpose of M and I is the n X n identity
 *          matrix).
 * https://oeis.org/A071306
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(2k+1) = 7^k * ∏_{i=0..k-1} (7^(2k) - 7^(2i)) a(2k) = (7^k +
 *            (-1)^(k+1)) * ∏_{i=1..k-1} (7^(2k) - 7^(2i)) (MacWilliams,
 *            1969). a(1)=1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* multiply base^exp using binary exponentiation */
static seq_t pow_seq(seq_t base, idx_t exp) {
    seq_t result = (seq_t)1;
    while (exp) {
        if (exp & (idx_t)1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    if (n <= 1) return (seq_t)1;          /* a(1) = 1 */

    bool odd = (n & (idx_t)1);
    idx_t k   = odd ? ((n - (idx_t)1) >> 1) : (n >> 1);

    /* 7^(2k) */
    seq_t pow2k = pow_seq((seq_t)7, (idx_t)2 * k);
    seq_t product = (seq_t)1;

    if (odd) {                            /* a(2k+1) case */
        seq_t pow2i = (seq_t)1;            /* 7^(0) */
        for (idx_t i = 0; i < k; ++i) {
            seq_t term = pow2k - pow2i;    /* term = 7^(2k) - 7^(2i) */
            product *= term;
            pow2i *= (seq_t)49;            /* step to next 7^(2(i+1)) */
        }
        seq_t coeff = pow_seq((seq_t)7, k);/* 7^k */
        return coeff * product;
    } else {                              /* a(2k) case */
        seq_t coeff = pow_seq((seq_t)7, k);/* 7^k */
        if ((k & (idx_t)1) == 0) coeff -= (seq_t)1; /* k even: subtract 1 */
        else                     coeff += (seq_t)1; /* k odd : add 1 */

        if (k == (idx_t)1)                         /* empty product */
            return coeff * product;

        seq_t pow2i = (seq_t)49;                    /* 7^(2*1) */
        for (idx_t i = 1; i < k; ++i) {
            seq_t term = pow2k - pow2i;            /* term = 7^(2k) - 7^(2i) */
            product *= term;
            pow2i *= (seq_t)49;                    /* step to next power */
        }
        return coeff * product;
    }
}
