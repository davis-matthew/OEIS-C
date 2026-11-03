/*
 * A024256  Expansion of log(1+sinh(x)*x)/2.
 * https://oeis.org/A024256
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n - Σ_{k=1}^{n-1} ( 2k * binomial(2n,2k) * (n-k)/n ) *
 *            a(n-k) , a(0)=0
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 7, i64 n <= 11, i128 n <= 14,
 *            big any n
 * Validated: partial, pass, 15 terms through n = 14 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

seq_t a(idx_t n)
{
    static seq_t *seq = NULL;
    static idx_t computed = 0;
    static size_t cap = 0;

    if (n + 1 > cap) {
        seq = seq_realloc(seq, cap, n + 1);
        cap = n + 1;
        computed = 0;
        seq[0] = 0;
    }

    if (n > computed) {
        for (idx_t i = computed + 1; i <= n; ++i) {
            if (i == 0) {
                seq[0] = 0;
                continue;
            }
            seq_t val = (seq_t)i;   /* start with n */
            seq_t C   = 1;          /* binomial(2*i,0) */

            for (idx_t k = 1; k <= i - 1; ++k) {
                /* update C to binomial(2*i, 2*k) */
                C = mul_small(C, (unsigned)(2 * i - (2 * k - 2)));
                C = mul_small(C, (unsigned)(2 * i - (2 * k - 1)));
                unsigned denomSmall = (unsigned)((2 * k - 1) * (2 * k));
                C = C / (seq_t)denomSmall;

                seq_t temp = mul_small(C, (unsigned)(i - k));
                temp = mul_small(temp, (unsigned)(2 * k));
                temp = temp / (seq_t)i;          /* exact division */

                seq_t prod = temp * seq[i - k];
                val -= prod;
            }
            seq[i] = val;
        }
        computed = n;
    }

    return seq[n];
}
