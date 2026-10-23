/*
 * A137213  First differences of A137212.
 * https://oeis.org/A137213
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^n - Pell_{n+1} Pell sequence: Pell_0 = 0, Pell_1 = 1,
 *            Pell_k = 2 * Pell_{k-1} + Pell_{k-2} for k ≥ 2 (the series
 *            1/(1-2x-x^2) generates Pell_{n+1})
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n) {
    /* static caches for powers of 3 and Pell numbers */
    static seq_t *pow3 = NULL;
    static size_t cap_pow3 = 0;
    static idx_t max_pow3 = 0;   /* greatest index computed in pow3 */

    static seq_t *pell = NULL;
    static size_t cap_pell = 0;
    static idx_t max_pell = 0;   /* greatest index computed in pell */

    /* initialise caches on first call */
    if (!pow3) {
        cap_pow3 = 1;
        pow3 = seq_realloc(NULL, 0, cap_pow3);
        pow3[0] = (seq_t)1;            /* 3^0 = 1 */
        max_pow3 = 0;
    }
    if (!pell) {
        cap_pell = 2;
        pell = seq_realloc(NULL, 0, cap_pell);
        pell[0] = (seq_t)0;            /* Pell_0 = 0 */
        pell[1] = (seq_t)1;            /* Pell_1 = 1 */
        max_pell = 1;
    }

    /* grow power‑of‑3 cache if needed */
    if (max_pow3 < n) {
        size_t new_cap = cap_pow3 ? cap_pow3 : 1;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *p = seq_realloc(pow3, cap_pow3, new_cap);
        pow3 = p;   cap_pow3 = new_cap;
        for (idx_t i = max_pow3 + 1; i <= n; ++i) {
            pow3[i] = pow3[i-1] * (seq_t)3;
        }
        max_pow3 = n;
    }

    /* grow Pell cache if needed (up to n+1) */
    idx_t target = n + 1;
    if (max_pell < target) {
        size_t new_cap = cap_pell ? cap_pell : 2;
        while (new_cap <= target) new_cap <<= 1;
        seq_t *p = seq_realloc(pell, cap_pell, new_cap);
        pell = p;   cap_pell = new_cap;
        for (idx_t i = max_pell + 1; i <= target; ++i) {
            pell[i] = pell[i-1] * (seq_t)2 + pell[i-2];
        }
        max_pell = target;
    }

    /* compute a(n) = 3^n - Pell_{n+1} */
    seq_t result = pow3[n] - pell[n+1];
    return result;
}
