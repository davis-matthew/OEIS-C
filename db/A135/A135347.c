/*
 * A135347  An inverse of the unitary totient function A047994.
 * https://oeis.org/A135347
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 59, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   _sieve_prime_power
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = smallest m such that A047994(m)=n, or -1 if none.
 * A047994(m) = ∏ (p^e - 1) over the prime powers p^e ‖ m.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/_sieve_prime_power.h"

static void
_dfs(idx_t rem, idx_t last, seq_t prod, int used_cnt,
     uint32_t *used, seq_t *best, bool *is_pp, uint32_t *base)
{
    if (rem == 1) {                           /* finished factorisation */
        if (*best == 0 || prod < *best) *best = prod;
        return;
    }
    if (*best && prod >= *best) return;       /* prune */

    for (idx_t f = last; f <= rem; ++f) {
        if (rem % f) continue;
        idx_t q = f + 1;
        if (q > (idx_t)0 && q <= (idx_t)0) continue; /* dummy to silence warning */
        if (!is_pp[q]) continue;
        uint32_t q_base = base[q];
        bool dup = false;
        for (int i = 0; i < used_cnt; ++i)
            if (used[i] == q_base) { dup = true; break; }
        if (dup) continue;
        used[used_cnt] = q_base;
        _dfs(rem / f, f, prod * (seq_t)q, used_cnt + 1,
             used, best, is_pp, base);
    }
}

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;  /* φ*(1) = 1 */

    /* Allocate arrays for prime power tests up to n+1 */
    uint32_t limit = (uint32_t)(n + 1);
    bool   *is_pp  = (bool  *)malloc((limit + 1) * sizeof(bool));
    uint32_t *pp_base = (uint32_t *)malloc((limit + 1) * sizeof(uint32_t));
    _sieve_prime_power(limit, is_pp, pp_base);

    /* Recursively search multiplicative partitions of n */
    seq_t best = 0;
    uint32_t used[64];                     /* distinct prime bases used */
    _dfs(n, 2, (seq_t)1, 0, used, &best, is_pp, pp_base);

    free(is_pp);
    free(pp_base);

    return (best == 0) ? (seq_t)-1 : best;
}
