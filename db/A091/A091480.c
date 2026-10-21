/*
 * A091480  Table of multigraphs (by antidiagonals) with n (>=1) nodes and k
 *          (>=0) edges. Each type of object labeled from its own label set.
 * https://oeis.org/A091480
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = binomial(n_node,2)^k, where (n_node,k) are the *
 *            parameters of the entry obtained by reading the table of
 *            multigraphs * along antidiagonals (increasing sum n_node-1 + k,
 *            and within each * diagonal increasing n_node). The function
 *            computes n_node and k from * the one‑dimensional index n
 * Range:     u8 n <= 31, u16 n <= 49, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"   /* binomial(idx_t a, idx_t b)             */
#include "../utils/big_pow.h"    /* big_pow(seq_t base, seq_t exp)        */

/* cumulative number of terms up to (and including) diagonal s.
   For diagonal s, there are s+1 entries, so C(s) = (s+1)(s+2)/2. */
static inline __uint128_t cumulative(idx_t s)
{
    return (__uint128_t)(s + 1) * (s + 2) / 2;
}

seq_t a(idx_t n)
{
    /* 1. Locate the diagonal s such that C(s-1) < n <= C(s).  */
    idx_t lo = 0, hi = 1;
    while (cumulative(hi) < n) {
        hi <<= 1;
        if (hi == 0) {          /* overflow protection */
            hi = ~(idx_t)0;
            break;
        }
    }
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        if (cumulative(mid) >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t s = lo;               /* diagonal index (starting from 0) */

    /* 2. Compute the position within that diagonal (0‑based). */
    idx_t prev = (idx_t)((s == 0) ? 0 : cumulative(s - 1));
    idx_t pos   = n - prev - 1; /* 0 <= pos <= s */

    /* 3. Determine (n_node,k).  n_node = pos + 1,   k = s - pos. */
    idx_t n_node = pos + 1;
    idx_t k      = s - pos;

    /* 4. Evaluate the term: binomial(n_node,2)^k. */
    seq_t base = binomial(n_node, 2);
    seq_t exp  = (seq_t)k;
    return big_pow(base, exp);
}
