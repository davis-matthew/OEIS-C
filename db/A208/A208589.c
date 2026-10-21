/*
 * A208589  Expansion of phi(x) / psi(x^4) in powers of x where phi(), psi()
 *          are Ramanujan theta functions.
 * https://oeis.org/A208589
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of q^n in φ(q)/ψ(q⁴) where φ(q) = Σ_{k∈ℤ}
 *            q^{k²} (Ramanujan theta function) ψ(q) = Σ_{k≥0} q^{k(k+1)/2}
 *            (Ramanujan theta function)
 * Range:     i8 n <= 56, i16 n <= 172, i32 n <= 568, i64 n <= 1992,
 *            i128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Helper: test whether n is a perfect square (returns 1 if true, 0 otherwise) */
static inline int is_square(idx_t n) {
    if (n == 0) return 1;
    idx_t low = 1, high = n;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == n) return 1;
        if (sq < n)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return 0;
}

/* Sequence generator */
seq_t a(idx_t n) {
    /* Static cache for computed coefficients */
    static seq_t *buf = NULL;
    static idx_t buf_size = 0;
    static idx_t next_compute = 0;   /* index from which to continue computation */

    /* Ensure buffer large enough */
    if (n + 1 > buf_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(buf, new_size * sizeof(seq_t));
        if (!tmp) abort();          /* allocation failure */
        buf = tmp;
        buf_size = new_size;
    }

    /* Compute coefficients up to n */
    for (idx_t i = next_compute; i <= n; ++i) {
        seq_t Fn;
        if (i == 0)
            Fn = 1;                       /* φ(0) = 1   */
        else
            Fn = is_square(i) ? 2 : 0;    /* φ(k) = 2  if k is a non‑zero square, else 0 */

        seq_t sum = 0;
        for (idx_t m = 1;; ++m) {
            /* ψ(q⁴) coefficient k = 2*m*(m+1)  (k > 0) */
            unsigned __int128 tmpk = (unsigned __int128)2 * m * (m + 1);
            if (tmpk > i) break;
            idx_t k = (idx_t)tmpk;
            sum += buf[i - k];
        }
        buf[i] = Fn - sum;   /* H[n] = Φ[n] – Σ G[k] * H[n‑k]  with G[0]=1, G[k]∈{0,1} */
    }
    next_compute = n + 1;
    return buf[n];
}
