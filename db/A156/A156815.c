/*
 * A156815  Triangle T(n, k) = n!*StirlingS2(n, k)/binomial(n, k), read by
 *          rows.
 * https://oeis.org/A156815
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,c) where * r = floor((sqrt(8n+1)-1)/2), c = n -
 *            r(r+1)/2, * T(r,c) = r! * StirlingS2(r,c) / binomial(r,c) * =
 *            StirlingS2(r,c) * c! * (r-c)!
 * Range:     u8 n <= 17, u16 n <= 37, u32 n <= 68, u64 n <= 156,
 *            u128 n <= 356, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* find row r and column c of flattened triangle */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) ++r;
    idx_t c = n - r * (r + 1) / 2;

    /* ---------- factorial cache ------------------------------------- */
    static seq_t *fact = NULL;
    static idx_t fact_size = 0;          /* largest n with fact[n] computed */
    if (!fact) {
        fact = (seq_t *)malloc(sizeof(seq_t));
        if (!fact) return (seq_t)0;
        fact[0] = 1;
        fact_size = 0;
    }
    if (r > fact_size) {
        fact = (seq_t *)realloc(fact, (r + 1) * sizeof(seq_t));
        if (!fact) return (seq_t)0;
        for (idx_t i = fact_size + 1; i <= r; ++i)
            fact[i] = fact[i - 1] * (seq_t)i;
        fact_size = r;
    }

    /* ---------- Stirling S2 cache ----------------------------------- */
    static seq_t **stab = NULL;
    static idx_t stab_rows = 0;          /* largest n with stab[n] allocated */
    if (!stab) {
        stab = (seq_t **)malloc(sizeof(seq_t *));
        if (!stab) return (seq_t)0;
        stab[0] = (seq_t *)malloc(sizeof(seq_t));
        if (!stab[0]) return (seq_t)0;
        stab[0][0] = 1;
        stab_rows = 0;
    }
    if (r > stab_rows) {
        stab = (seq_t **)realloc(stab, (r + 1) * sizeof(seq_t *));
        if (!stab) return (seq_t)0;
        for (idx_t i = stab_rows + 1; i <= r; ++i) {
            stab[i] = (seq_t *)malloc((i + 1) * sizeof(seq_t));
            if (!stab[i]) return (seq_t)0;
            stab[i][0] = 0;
            stab[i][i] = 1;
            for (idx_t j = 1; j < i; ++j) {
                seq_t t1 = stab[i - 1][j] * (seq_t)j;
                seq_t t2 = stab[i - 1][j - 1];
                stab[i][j] = t1 + t2;
            }
        }
        stab_rows = r;
    }

    /* ---------- compute T(r,c) = fact[c] * fact[r-c] * S2(r,c) ------- */
    seq_t val = fact[c];
    val = val * fact[r - c];
    val = val * stab[r][c];
    return val;
}
