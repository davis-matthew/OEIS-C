/*
 * A089735  Self-convolution of A004148 (the RNA secondary structure numbers)
 *          with itself.
 * https://oeis.org/A089735
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*Sum_{k=ceil((n+1)/2)..n}
 *            binomial(k,n-k)*binomial(k+1,n-k+2)/k The sequence satisfies the
 *            recurrence for n>=6: (n+4)*a(n) +(-3*n-7)*a(n-1) +2*n*a(n-2)
 *            +3*(-n+1)*a(n-3) +2*(n-2)*a(n-4) +(-3*n+13)*a(n-5)
 *            +(n-6)*a(n-6)=0 with initial values a(0..5) = 1, 2, 3, 6, 13,
 *            28. This implementation...
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   mul_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>


#include "../utils/mul_seq_t.h"  /* for mul_seq_t */

static seq_t *A = NULL;      /* cached sequence values */
static size_t A_len = 0;      /* number of allocated slots  */
static idx_t  A_next = 0;     /* next index that needs to be computed */

/* convenience array with the six seed values */
static const seq_t seed[6] = {
    (seq_t)1,  /* a(0) */
    (seq_t)2,  /* a(1) */
    (seq_t)3,  /* a(2) */
    (seq_t)6,  /* a(3) */
    (seq_t)13, /* a(4) */
    (seq_t)28  /* a(5) */
};

/* ensure that A[0..n] are computed and stored in the cache */
static void ensure_A(idx_t n)
{
    if (A_len > n) return;                    /* already present */

    /* grow array to n+1 elements (index 0..n) */
    size_t new_len = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(A, new_len * sizeof(seq_t));
    if (!tmp) return;   /* allocation failure – caller will fail */
    A = tmp;
    /* initialise new slots to zero */
    for (size_t i = A_len; i < new_len; ++i) A[i] = (seq_t)0;

    /* compute missing values */
    for (idx_t i = A_next; i <= (idx_t)n; ++i)
    {
        if (i <= 5)                       /* use seed values for 0..5 */
        {
            A[i] = seed[(size_t)i];
        }
        else
        {
            /* recurrence for n >= 6 */
            const idx_t ii = i;
            /* coefficients (may be negative) */
            long long c1 = -3LL * (long long)ii - 7;          /* for a(n-1) */
            long long c2 =  2LL * (long long)ii;             /* for a(n-2) */
            long long c3 =  3LL * (-(long long)ii + 1);     /* for a(n-3) */
            long long c4 =  2LL * (long long)(ii - 2);       /* for a(n-4) */
            long long c5 = -3LL * (long long)ii + 13;         /* for a(n-5) */
            long long c6 =  (long long)ii - 6;               /* for a(n-6) */

            seq_t pos_sum = (seq_t)0;   /* sum of positive coefficient terms */
            seq_t neg_sum = (seq_t)0;   /* sum of negative coefficient terms */

            /* helper macro to add a term contributed by coefficient ci and a(prev) */
#define ADD_TERM(ci, prev_idx)                                 \
            do {                                               \
                long long ci_val = (ci);                       \
                uint64_t abs_coeff = (ci_val > 0) ? ci_val : -(ci_val); \
                seq_t term = mul_seq_t(A[(size_t)(prev_idx)], (seq_t)abs_coeff); \
                if (ci_val > 0) {                             \
                    pos_sum += term;                          \
                } else {                                      \
                    neg_sum += term;                          \
                }                                             \
            } while (0)

            ADD_TERM(c1, ii - 1);
            ADD_TERM(c2, ii - 2);
            ADD_TERM(c3, ii - 3);
            ADD_TERM(c4, ii - 4);
            ADD_TERM(c5, ii - 5);
            ADD_TERM(c6, ii - 6);

#undef ADD_TERM

            /* numerator = neg_sum - pos_sum (guaranteed non‑negative) */
            seq_t numerator = neg_sum - pos_sum;

            /* denominator = ii + 4 */
            seq_t denom = (seq_t)(ii + 4);

            /* exact division, result fits in seq_t */
            A[i] = numerator / denom;
        }
    }

    A_next = (idx_t)n + 1;
    A_len   = new_len;
}

seq_t a(idx_t n)
{
    ensure_A(n);
    return A[n];
}
