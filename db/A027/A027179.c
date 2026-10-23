/*
 * A027179  a(n) = T(n,0) + T(n,1) + ... + T(n,[ n/2 ]), T given by A027170.
 * https://oeis.org/A027179
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 18, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
 * (n+2)*(3*n-20)*a(n) = -(n+1)*(3*n-11)*a(n-1) +
 *       (33*n^2-180*n-100)*a(n-2) +
 *       (15*n^2-56*n+1)*a(n-3) +
 *       2*(-51*n^2+313*n-208)*a(n-4) -
 *       12*(n-1)*(n-4)*a(n-5) +
 *       8*(9*n-11)*(n-5)*a(n-6) +
 *       4*(-24*n^2+196*n-365)
 * Initial values: a(0)=1, a(1)=3, a(2)=15, a(3)=26, a(4)=81, a(5)=130.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;
static const seq_t base_vals[6] = {1, 3, 15, 26, 81, 130};

typedef struct { seq_t val; bool neg; } signed_seq_t;

static void add_term(signed_seq_t *s, seq_t x) {
    if (s->neg) {
        if (x > s->val) {
            s->val = x - s->val;
            s->neg = false;
        } else if (x < s->val) {
            s->val = s->val - x;
            s->neg = true;
        } else {
            s->val = 0;
            s->neg = false;
        }
    } else {
        s->val += x;
    }
}

static void sub_term(signed_seq_t *s, seq_t x) {
    if (s->neg) {
        s->val += x;
    } else {
        if (x > s->val) {
            s->val = x - s->val;
            s->neg = true;
        } else if (x < s->val) {
            s->val -= x;
        } else {
            s->val = 0;
            s->neg = false;
        }
    }
}

seq_t a(idx_t n) {
    if (n < 6) return base_vals[n];

    if (!cache) {
        cache_size = 6;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        for (size_t i = 0; i < 6; ++i) cache[i] = base_vals[i];
    }

    if (n >= cache_size) {
        size_t new_size = (size_t)(n + 1);
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));

        for (idx_t k = cache_size; k <= n; ++k) {
            signed_seq_t sum = {0, false};
            int64_t K = (int64_t)k;

            /* term 1: -(K+1)*(3K-11) */
            int64_t c1 = - (K + 1) * (3 * K - 11);
            seq_t coeff1 = (c1 >= 0 ? (seq_t)c1 : (seq_t)(-c1));
            if (c1 < 0) sub_term(&sum, coeff1 * cache[k - 1]);
            else add_term(&sum, coeff1 * cache[k - 1]);

            /* term 2: +(33K^2-180K-100) */
            int64_t c2 = 33 * K * K - 180 * K - 100;
            seq_t coeff2 = (c2 >= 0 ? (seq_t)c2 : (seq_t)(-c2));
            if (c2 < 0) sub_term(&sum, coeff2 * cache[k - 2]);
            else add_term(&sum, coeff2 * cache[k - 2]);

            /* term 3: +(15K^2-56K+1) */
            int64_t c3 = 15 * K * K - 56 * K + 1;
            seq_t coeff3 = (c3 >= 0 ? (seq_t)c3 : (seq_t)(-c3));
            if (c3 < 0) sub_term(&sum, coeff3 * cache[k - 3]);
            else add_term(&sum, coeff3 * cache[k - 3]);

            /* term 4: +(2*(-51K^2+313K-208)) */
            int64_t c4 = 2 * (-51 * K * K + 313 * K - 208);
            seq_t coeff4 = (c4 >= 0 ? (seq_t)c4 : (seq_t)(-c4));
            if (c4 < 0) sub_term(&sum, coeff4 * cache[k - 4]);
            else add_term(&sum, coeff4 * cache[k - 4]);

            /* term 5: -12*(K-1)*(K-4) */
            int64_t c5 = -12 * (K - 1) * (K - 4);
            seq_t coeff5 = (c5 >= 0 ? (seq_t)c5 : (seq_t)(-c5));
            if (c5 < 0) sub_term(&sum, coeff5 * cache[k - 5]);
            else add_term(&sum, coeff5 * cache[k - 5]);

            /* term 6: +8*(9K-11)*(K-5) */
            int64_t c6 = 8 * (9 * K - 11) * (K - 5);
            seq_t coeff6 = (c6 >= 0 ? (seq_t)c6 : (seq_t)(-c6));
            if (c6 < 0) sub_term(&sum, coeff6 * cache[k - 6]);
            else add_term(&sum, coeff6 * cache[k - 6]);

            /* constant term: 4*(-24K^2+196K-365) */
            int64_t c7 = 4 * (-24 * K * K + 196 * K - 365);
            seq_t coeff7 = (c7 >= 0 ? (seq_t)c7 : (seq_t)(-c7));
            if (c7 < 0) sub_term(&sum, coeff7);
            else add_term(&sum, coeff7);

            /* denominator */
            int64_t denom_signed = (K + 2) * (3 * K - 20);
            seq_t abs_den = (seq_t)(denom_signed < 0 ? -denom_signed : denom_signed);

            cache[k] = sum.val / abs_den;
        }
        cache_size = cache_size = (idx_t)n + 1;
    }
    return cache[n];
}
