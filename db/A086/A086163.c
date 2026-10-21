/*
 * A086163  Number of monomial ideals in two variables x, y that are Artinian,
 *          integrally closed, of colength n and contain x^4.
 * https://oeis.org/A086163
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 47, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A086163: Number of monomial ideals in two variables x,y that
 * are Artinian, integrally closed, of colength n and contain x^4.
 *
 * The sequence is given by the generating function
 *
 *   G(t) = (1 + t^2 + t^4 + t^5 - t^6 - t^7 + 2 t^9 - 2 t^10 - t^11
 *          - 2 t^12 + 2 t^13 - t^14 - t^15 + t^16 + t^17 + t^18 - t^19)
 *          / ((1-t)(1-t^3)(1-t^6)(1-t^10)).
 *
 *   Let q(k) be the coefficient of t^k in
 *          1/((1-t)(1-t^3)(1-t^6)(1-t^10)),
 *   i.e. the number of partitions of k into parts 1,3,6 and 10.
 *
 *   Then
 *      a(n) = Σ_{j=0}^{19} coeff[j] · q(n-j),
 *   where coeff[j] are the coefficients of the numerator polynomial.
 *
 *   The q(k) are computed by the standard coin‐change DP:
 *      q[0] = 1;
 *      for each part s in {1,3,6,10}
 *          for i = s .. N
 *              q[i] += q[i-s].
 *
 *   The routine is memoised; it handles calls in any order.
 */

#include <stdlib.h>

/* Numerator coefficients of G(t).  coeff[j] is the coefficient of t^j. */
static const int coeff[20] = {
    1,  0,  1,  0,  1,  1,
   -1, -1,  0,  2, -2, -1,
   -2,  2, -1, -1,  1,  1,
    1, -1
};

/* Parts used in the denominator: 1,3,6,10. */
static const idx_t parts[4] = { 1, 3, 6, 10 };

/* Cached values of q(k). */
static seq_t *q_cache = NULL;   /* array of q[0..] */
static size_t q_len  = 0;        /* number of elements stored (*actual* length) */
static size_t q_cap  = 0;        /* allocated capacity of q_cache */

/* Ensure that q_cache[0..n] are computed. */
static void ensure_q(idx_t n)
{
    /* Initialise or enlarge the cache if necessary. */
    size_t new_len = (size_t)n + 1;
    if (q_cap < new_len) {
        size_t new_cap = (q_cap > 0) ? q_cap * 2 : 16;
        if (new_cap < new_len) new_cap = new_len;
        seq_t *tmp = (seq_t *)realloc(q_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        q_cache = tmp;
        q_cap   = new_cap;
    }

    /* If the cache already contains enough entries, nothing to do. */
    if (q_len >= new_len) return;

    /* Set all entries to 0 and initialise q[0] = 1. */
    for (size_t i = q_len; i < new_len; ++i) q_cache[i] = (seq_t)0;
    if (q_len == 0) q_cache[0] = (seq_t)1;
    /* Compute q values up to n using the coin‑change DP. */
    for (size_t i = 0; i < new_len; ++i) q_cache[i] = (seq_t)0;
    q_cache[0] = (seq_t)1;
    for (size_t pc = 0; pc < 4; ++pc) {
        idx_t part = parts[pc];
        for (size_t i = (size_t)part; i < new_len; ++i) {
            q_cache[i] += q_cache[i - part];
        }
    }
    q_len = new_len;
}

/* Public function required by the exercise. */
seq_t a(idx_t n)
{
    ensure_q(n);              /* make sure q[0..n] are available */

    seq_t res = (seq_t)0;
    size_t jmax = 20;
    if (jmax > (size_t)n + 1) jmax = (size_t)n + 1;
    for (size_t j = 0; j < jmax; ++j) {
        res += (seq_t)coeff[j] * q_cache[n - j];
    }
    return res;
}
