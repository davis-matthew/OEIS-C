/*
 * A069957  Expansion of 1/((1-x)^2*(1-x^2)^2*(1-x^3)*(1-x^4)^2*(1-x^5)).
 * https://oeis.org/A069957
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = -∑_{k=1}^{22} d[k] * a(n-k) for n ≥ 1, a(0)=1 (where
 *            d[0..22] are the coefficients of
 *            (1-x)^2*(1-x^2)^2*(1-x^3)*(1-x^4)^2*(1-x^5) )
 * Range:     u8 n <= 9, u16 n <= 33, u32 n <= 203, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Coefficients of the denominator polynomial D(x)=∑_{k=0}^{22} d[k]x^k.
   d[0] is 1 (not used in the recurrence). */
static const int coeff[23] = {
    0,  /* d[0] unused */
    -2,  /* d[1] */
    -1,  /* d[2] */
    3,   /* d[3] */
    -1,  /* d[4] */
    2,   /* d[5] */
    1,   /* d[6] */
    -4,  /* d[7] */
    -2,  /* d[8] */
    0,   /* d[9] */
    2,   /* d[10] */
    2,   /* d[11] */
    2,   /* d[12] */
    0,   /* d[13] */
    -2,  /* d[14] */
    -4,  /* d[15] */
    1,   /* d[16] */
    2,   /* d[17] */
    -1,  /* d[18] */
    3,   /* d[19] */
    -1,  /* d[20] */
    -2,  /* d[21] */
    1    /* d[22] */
};

seq_t a(idx_t n)
{
    /* Static storage for already computed terms. */
    static seq_t *memo = NULL;  /* array of computed a(i) */
    static idx_t memo_len = 0;  /* number of stored terms (n+1) */
    static idx_t memo_cap = 0;  /* allocated capacity */

    /* Initialize storage and first value. */
    if (memo == NULL) {
        memo_cap = 64;                     /* initial capacity */
        memo = (seq_t *)malloc(memo_cap * sizeof(seq_t));
        if (!memo) return (seq_t)0;         /* allocation failed */
        memo[0] = (seq_t)1;                 /* a(0) */
        memo_len = 1;
    }

    /* If requested value is already known, return it. */
    if (n < memo_len)
        return memo[n];

    /* Ensure capacity for up to n. */
    if (n + 1 > memo_cap) {
        idx_t new_cap = memo_cap ? memo_cap * 2 : 64;
        while (new_cap <= n)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(memo, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* allocation failed */
        memo = tmp;
        memo_cap = new_cap;
    }

    /* Compute missing terms up to n. */
    for (idx_t i = memo_len; i <= n; ++i) {
        if (i == 0) {                       /* already initialized */
            memo[i] = (seq_t)1;
            continue;
        }
        seq_t pos = (seq_t)0;
        seq_t neg = (seq_t)0;
        for (int k = 1; k <= 22 && (idx_t)k <= i; ++k) {
            int c = coeff[k];
            seq_t val = memo[i - k];
            if (c > 0)
                pos += val * (seq_t)c;
            else if (c < 0)
                neg += val * (seq_t)(-c);
        }
        /* a(i) = -(sum) = neg - pos  (since sum = pos - neg) */
        memo[i] = neg - pos;
    }

    memo_len = n + 1;
    return memo[n];
}
