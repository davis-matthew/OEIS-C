/*
 * A075733  Array of coefficients in Zagier's polynomials P_(n,0)(x).
 * https://oeis.org/A075733
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = coefficient of the polynomial P_(n,0)(x) = (-1)^n *
 *            2^(2n-1) * ∏_{i=1..n} (x – cos(π(2i–1)/(4n))^2). Coefficients
 *            are listed in order of P_1, then P_2, …, i.e. a(1),a(2) are the
 *            coefficients of P_(1,0)(x), a(3),a(4),a(5) are the coefficients
 *            of P_(2,0)(x), etc
 * Range:     i8 n <= 11, i16 n <= 32, i32 n <= 51, i64 n <= 51, i128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* External integer types supplied by the build */
extern seq_t a(idx_t n);

seq_t a(idx_t n) {
    if (n == 0) return 0; /* undefined below offset, but safe */

    /* ----------------------- static state ----------------------- */
    /* Even‑coefficient tables: even_coefs[i] holds the array
       of coefficients of P_(i+1,0)(x) (length i+2) */
    static seq_t **even_coefs = NULL;
    static size_t even_cap = 0;        /* how many n’s are precomputed */
    static size_t even_alloc = 0;      /* allocated capacity for even_coefs */

    /* Chebyshev state: T_{k-1} and T_k */
    static seq_t *T_prev = NULL;
    static size_t T_prev_len = 0;
    static seq_t *T_curr = NULL;
    static size_t T_curr_len = 0;
    static idx_t  T_k = 1; /* current k of T_curr (starts at 1) */

    /* ---- initialize Chebyshev arrays on first call ---- */
    if (even_cap == 0) {
        /* T_0 */
        T_prev_len = 1;
        T_prev = (seq_t*)malloc(sizeof(seq_t) * T_prev_len);
        T_prev[0] = (seq_t)1;

        /* T_1 */
        T_curr_len = 2;
        T_curr = (seq_t*)malloc(sizeof(seq_t) * T_curr_len);
        T_curr[0] = (seq_t)0;
        T_curr[1] = (seq_t)1;
    }

    /* ------------------- determine polynomial index ------------------- */
    /* Find the smallest p such that p*(p+3)/2 >= n.
       This p is the index of the polynomial whose coefficients
       include a(n). */
    idx_t p = 1;
    while (((idx_t)p * (p + 3)) / 2 < n) ++p;

    /* ------------------- compute up to that polynomial ---------------- */
    while (even_cap < (size_t)p) {
        /* Compute next Chebyshev polynomial T_{k+1} = 2x*T_k - T_{k-1} */
        idx_t next_k    = T_k + 1;
        size_t next_len = T_curr_len + 1;
        seq_t *next = (seq_t*)malloc(sizeof(seq_t) * next_len);
        for (idx_t i = 0; i < next_len; ++i) next[i] = 0;

        /* 2x * T_k (shift left by one, multiply by 2) */
        for (idx_t i = 0; i < T_curr_len; ++i)
            next[i + 1] = (seq_t)2 * T_curr[i];

        /* Subtract T_{k-1} */
        for (idx_t i = 0; i < T_prev_len; ++i)
            next[i] = next[i] - T_prev[i];

        /* Rotate buffers */
        free(T_prev);
        T_prev = T_curr;
        T_prev_len = T_curr_len;
        T_curr = next;
        T_curr_len = next_len;
        T_k = next_k;

        /* If k is even, store coefficients of P_(k/2,0)(x) */
        if ((T_k & 1) == 0) {
            idx_t m = T_k >> 1;          /* polynomial index m = k/2  */
            seq_t *coef = (seq_t*)malloc(sizeof(seq_t) * (m + 1));
            seq_t sign = (m & 1) ? (seq_t)-1 : (seq_t)1;
            for (idx_t j = 0; j <= m; ++j)
                coef[j] = T_curr[2 * j] * sign;

            /* Resize storage if needed */
            if (even_cap >= even_alloc) {
                size_t new_alloc = even_alloc ? even_alloc * 2 : 8;
                even_coefs = (seq_t**)realloc(even_coefs,
                                              sizeof(seq_t*) * new_alloc);
                even_alloc = new_alloc;
            }
            even_coefs[even_cap++] = coef;
        }
    }

    /* ------------------- retrieve the requested term ---------------- */
    idx_t prev_total = ((idx_t)(p - 1) * (p + 2)) / 2;
    idx_t pos = n - prev_total - 1;          /* 0‑based position inside the polynomial */
    return even_coefs[p - 1][pos];
}
