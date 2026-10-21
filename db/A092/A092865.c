/*
 * A092865  Nonzero elements in Klee's identity Sum[(-1)^k
 *          binomial[n,k]binomial[n+k,m],{k,0,n}] == (-1)^n binomial[n,m-n].
 * https://oeis.org/A092865
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabf
 * Formula:   a(n) enumerates the non‑zero entries of the Klee identity
 *            Sum[(-1)^k binomial[n,k] binomial[n+k,m],{k,0,n}] == (-1)^n
 *            binomial[n,m-n] in order of increasing m then n; hence a(n) =
 *            (-1)^n_val * binomial[n_val, m-n_val] where (m, offset)
 *            satisfies the cumulative index condition
 * Range:     i8 n <= 37, i16 n <= 76, i32 n <= 76, i64 n <= 76, i128 n <= 76,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t idx) {
    /* Find the anti‑diagonal index m such that the cumulative
       number of terms up to m exceeds idx. */
    idx_t m = 0;
    for (;;) {
        idx_t w = m / 2;
        idx_t t;
        if ((m & 1) == 0)
            t = (w + 1) * (w + 1);             /* (floor(m/2)+1)²   for even m */
        else
            t = (w + 1) * (w + 2);             /* (floor(m/2)+1)*(floor(m/2)+2) for odd m */
        if (t > idx) break;
        ++m;
    }

    /* Cumulative count up to the previous anti‑diagonal (m-1). */
    idx_t t_prev = 0;
    if (m > 0) {
        idx_t m_prev = m - 1;
        idx_t w_prev = m_prev / 2;
        if ((m_prev & 1) == 0)
            t_prev = (w_prev + 1) * (w_prev + 1);
        else
            t_prev = (w_prev + 1) * (w_prev + 2);
    }

    idx_t offset = idx - t_prev;                 /* position within this anti‑diagonal */
    idx_t n_val = ((m + 1) / 2) + offset;       /* ceil(m/2) + offset = n */
    idx_t k = m - n_val;                         /* k = m - n */

    seq_t val = binom(n_val, k);
    if ((n_val & 1) != 0)
        return -val;
    else
        return val;
}
