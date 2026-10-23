/*
 * A159226  Number of n X n arrays of squares of integers with every 2X2
 *          subblock summing to 26.
 * https://oeis.org/A159226
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = -56*2^(n/2-1) + 36*2^(n-1) + 2^(3*n/2+2) - 6 if n is even
 *            -84*2^((n-1)/2-1) + 38*2^(n-1) + 12*8^((n-1)/2) - 6 if n is odd
 *            These expressions are algebraically equivalent to the OEIS
 *            empirical formula and yield integer values for all n ≥ 2. The
 *            function below evaluates them using only intege...
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 19, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h> /* for uint64_t if needed; seq_t and idx_t are defined elsewhere */

static seq_t pow2_seq(idx_t exp) {
    seq_t result = 1;
    while (exp > 0) {
        result *= (seq_t)2;
        exp--;
    }
    return result;
}

seq_t a(idx_t n) {
    if (n < 2) return 0;  /* offset not used */
    if ((n & 1) == 0) {  /* even n = 2*k */
        idx_t k = n >> 1;                 /* k = n/2 */
        seq_t pow_k_minus1  = pow2_seq(k - 1);
        seq_t pow_2k_minus1 = pow2_seq((k << 1) - 1);   /* 2*k - 1 */
        seq_t pow_3k_plus2 = pow2_seq((k * 3) + 2);     /* 3*k + 2 */

        seq_t res = pow_3k_plus2;        /* + 2^(3k+2) */
        res += (seq_t)36 * pow_2k_minus1;/* + 36*2^(2k-1) which equals 36*2^(n-1) */
        res -= (seq_t)56 * pow_k_minus1; /* - 56*2^(k-1)  */
        res -= (seq_t)6;                 /* -6 */
        return res;
    } else {                            /* odd n = 2*k+1, k>=1 */
        idx_t k = (n - 1) >> 1;         /* k = (n-1)/2 */
        seq_t pow_k_minus1 = pow2_seq(k - 1);   /* 2^(k-1) */
        seq_t pow_2k       = pow2_seq(k << 1);  /* 2^(2k)  */
        seq_t pow_3k       = pow2_seq(k * 3);    /* 2^(3k) = 8^k */

        seq_t res = (seq_t)12 * pow_3k;        /* +12*8^k */
        res += (seq_t)38 * pow_2k;             /* +38*2^(2k) = 38*2^(n-1) */
        res -= (seq_t)84 * pow_k_minus1;       /* -84*2^(k-1) */
        res -= (seq_t)6;                       /* -6 */
        return res;
    }
}
