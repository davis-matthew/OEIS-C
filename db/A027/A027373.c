/*
 * A027373  Coordination sequence T1 for Zeolite Code SAT.
 * https://oeis.org/A027373
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 184, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Coordination sequence T1 for Zeolite Code SAT (A027373)
 *  Recurrence derived from the denominator of the generating function.
 *  Initial terms (a(0)…a(21)) are taken directly from the OEIS entry;
 *  for n≥22 the sequence satisfies:
 *      a(n) = Σ_{k=1}^{21} (-q_k) * a(n-k)
 *  where the Q‑coefficients q_k come from
 *      Q(x) = (1−x)^3(1−x+x^2−x^3+x^4)(1+x^4)(1+x+x^2+x^3+x^4)(1+x^3+x^6)
 *  and are  q = [1, −3, 4, −3, 2, −3, 5, −6, 5, −4, 4, −4, 4, −5, 6, −5, 3, −2, 3, −4, 3, −1].
 *  Thus  (-q) = [3, −4, 3, −2, 3, −5, 6, −5, 4, −4, 4, −4, 5, −6, 5, −3, 2, −3, 4, −3, 1].
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

/* Precomputed first 22 terms */
static const seq_t init_terms[22] = {
    /* a(0) … a(21) */
    (seq_t)1, (seq_t)4, (seq_t)10, (seq_t)20, (seq_t)33, (seq_t)50,
    (seq_t)71, (seq_t)95, (seq_t)124, (seq_t)158, (seq_t)197, (seq_t)239,
    (seq_t)282, (seq_t)329, (seq_t)380, (seq_t)435, (seq_t)495, (seq_t)558,
    (seq_t)627, (seq_t)702, (seq_t)779, (seq_t)858
};

/* Q-coefficients q_k from the denominator expansion, q[0] = 1 */
static const int coeff_q[22] = {
    1,   -3,  4,  -3,  2,  -3,  5, -6, 5, -4,
    4,  -4,  4, -5,  6, -5,  3, -2, 3, -4,
    3,  -1
};

/* Cached sequence values */
static seq_t *cached = NULL;
static idx_t cache_len = 0;
static idx_t cache_cap = 0;

/* Ensure cache can hold at least n+1 elements */
static void ensure_cache(idx_t n)
{
    if (n + 1 > cache_cap) {
        idx_t new_cap = (cache_cap ? cache_cap * 2 : n + 1);
        if (new_cap < n + 1) new_cap = n + 1;
        seq_t *newp = (seq_t*)realloc(cached, new_cap * sizeof(seq_t));
        if (!newp) exit(1);  /* out of memory */
        cached = newp;
        cache_cap = new_cap;
    }
    if (cache_len == 0) {
        /* initialize first 22 terms */
        cache_len = 22;
        for (int i = 0; i < 22; ++i) cached[i] = init_terms[i];
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 22) return init_terms[(int)n];

    ensure_cache(n);

    /* Compute terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t pos = 0;
        seq_t neg = 0;
        /* recurrence: a(i) = Σ (-q_k) * a(i-k)
           split positive and negative contributions to stay unsigned */
        for (int k = 1; k <= 21; ++k) {
            idx_t idx = i - k;
            seq_t term = cached[idx] * (seq_t)(-(coeff_q[k] < 0 ? coeff_q[k] : -coeff_q[k]));
            /* coeff_q[k] may be negative or positive */
            if (coeff_q[k] < 0) {
                /* -q_k positive contributes to pos */
                pos += term;
            } else {
                /* -q_k negative contributes to neg */
                neg += term;
            }
        }
        cached[i] = pos - neg; /* pos ≥ neg by construction */
    }

    cache_len = n + 1;  /* now we have values up to n */
    return cached[n];
}
