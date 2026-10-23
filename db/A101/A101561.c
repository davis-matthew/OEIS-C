/*
 * A101561  a(n) = (-1)^n * [x^n] Sum_{k>=1} x^(k-1)/(1+3*x^k).
 * https://oeis.org/A101561
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (-1)^n * Σ_{d|n+1} (-3)^(d-1)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses separate positive and negative sums to avoid
   using signed arithmetic.  Divisors are enumerated in O(√m) time.  */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* m = n + 1, so m >= 1 */
    idx_t m = n + 1;

    /* Memoized powers of 3 up to the largest exponent needed. */
    static seq_t *pow3 = NULL;
    static idx_t pow3_len = 0;

    /* The largest exponent we need is m-1. */
    idx_t need = m - 1;
    if (pow3_len == 0) {
        pow3 = (seq_t*)realloc(NULL, sizeof(seq_t));
        if (!pow3) return 0;            /* allocation failure – unlikely */
        pow3[0] = (seq_t)1;
        pow3_len = 1;
    }
    if (pow3_len <= need) {
        idx_t new_len = need + 1;
        seq_t *tmp = (seq_t*)realloc(pow3, new_len * sizeof(seq_t));
        if (!tmp) return 0;            /* allocation failure – unlikely */
        pow3 = tmp;
        for (idx_t i = pow3_len; i <= need; ++i)
            pow3[i] = pow3[i-1] * (seq_t)3;
        pow3_len = new_len;
    }

    /* Accumulate sums of terms with even/odd exponents. */
    seq_t pos_sum = 0; /* exponents d-1 even  → term positive */
    seq_t neg_sum = 0; /* exponents d-1 odd   → term negative (counted as positive here) */

    for (idx_t i = 1; i <= m / i; ++i) {
        if (m % i == 0) {
            /* divisor i */
            idx_t d = i;
            idx_t exp = d - 1;
            seq_t term = pow3[exp];
            if ((exp & 1) == 0)
                pos_sum += term;
            else
                neg_sum += term;

            /* divisor m / i, if distinct */
            idx_t d2 = m / i;
            if (d2 != d) {
                exp = d2 - 1;
                term = pow3[exp];
                if ((exp & 1) == 0)
                    pos_sum += term;
                else
                    neg_sum += term;
            }
        }
    }

    /* Final result: apply the outer (-1)^n factor. */
    if ((n & 1) == 0)
        return pos_sum - neg_sum;
    else
        return neg_sum - pos_sum;
}
