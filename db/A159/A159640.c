/*
 * A159640  a(1) = a(2) = 1; for n > 2, a(n) = (a(1), a(2), a(3), ...) dot
 *          (P(1), P(2), P(3), ...); P = A000129.
 * https://oeis.org/A159640
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = a(2) = 1; for n>2, a(n) = Σ_{j=1}^{n-1} a(j) * P(j) (Pell
 *            numbers: P(1)=1, P(2)=2, P(k)=2*P(k-1)+P(k-2))
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 8, u64 n <= 11, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* seq_t and idx_t are supplied by the build for the required integer types */
seq_t a(idx_t n) {
    /* Static caches for a(n), Pell p(n), and cumulative sums s(n) */
    static seq_t *a_cache   = NULL;
    static seq_t *p_cache   = NULL;
    static seq_t *s_cache   = NULL;
    static size_t a_cap = 0;   /* capacity (max index that can be stored) */
    static size_t a_len = 0;   /* largest index that has been computed */

/* ---------- initialization ---------- */
    if (a_len == 0) {
        /* Start with space for indices 0..2 (we ignore position 0) */
        size_t init_cap = 2;
        a_cache = (seq_t *)malloc((init_cap + 1) * sizeof(seq_t));
        p_cache = (seq_t *)malloc((init_cap + 1) * sizeof(seq_t));
        s_cache = (seq_t *)malloc((init_cap + 1) * sizeof(seq_t));
        if (!a_cache || !p_cache || !s_cache) {
            /* Failure is fatal for this helper */
            exit(1);
        }
        a_cap = init_cap;

        /* Base values */
        a_cache[1] = (seq_t)1;
        a_cache[2] = (seq_t)1;

        p_cache[1] = (seq_t)1;
        p_cache[2] = (seq_t)2;

        /* Cumulated sum s[k] = Σ_{j=1}^{k} a(j)*p(j) */
        s_cache[1] = a_cache[1] * p_cache[1];                       /* 1 */
        s_cache[2] = s_cache[1] + a_cache[2] * p_cache[2];          /* 1 + 2 = 3 */

        a_len = 2;
    }

/* ---------- handle small n ---------- */
    if (n == 0) {
        return (seq_t)0;   /* undefined, but return 0 for safety */
    }
    if (n <= a_len) {
        return a_cache[n];
    }

/* ---------- ensure capacity ---------- */
    if (n > a_cap) {
        size_t new_cap = a_cap ? a_cap * 2 : 4;
        while (new_cap < (size_t)n) {
            new_cap <<= 1;
        }
        seq_t *new_a = (seq_t *)realloc(a_cache, (new_cap + 1) * sizeof(seq_t));
        seq_t *new_p = (seq_t *)realloc(p_cache, (new_cap + 1) * sizeof(seq_t));
        seq_t *new_s = (seq_t *)realloc(s_cache, (new_cap + 1) * sizeof(seq_t));
        if (!new_a || !new_p || !new_s) {
            exit(1);
        }
        a_cache = new_a;
        p_cache = new_p;
        s_cache = new_s;
        a_cap   = new_cap;
    }

/* ---------- compute terms up to n ---------- */
    for (idx_t k = a_len + 1; k <= n; ++k) {
        /* Pell number */
        if (k == 1) {
            p_cache[k] = (seq_t)1;
        } else if (k == 2) {
            p_cache[k] = (seq_t)2;
        } else {
            p_cache[k] = (seq_t)((seq_t)2 * p_cache[k - 1] + p_cache[k - 2]);
        }

        /* a(k) = cumulative sum up to previous index */
        a_cache[k] = s_cache[k - 1];

        /* Update cumulative sum to include a(k)*p(k) */
        s_cache[k] = s_cache[k - 1] + a_cache[k] * p_cache[k];
    }

    a_len = (size_t)n;
    return a_cache[n];
}
