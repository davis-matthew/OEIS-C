/*
 * A045070  Numbers whose base-4 representation contains exactly one 0 and
 *          four 3's.
 * https://oeis.org/A045070
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   numbers whose base-4 representation contains exactly one 0 and
 *            four 3's
 * Range:     u8 none, u16 n <= 2374, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


/* Comparison function for qsort */
static int seq_compare(const void *a, const void *b)
{
    seq_t va = *(const seq_t*)a;
    seq_t vb = *(const seq_t*)b;
    return (va < vb) ? -1 : (va > vb) ? 1 : 0;
}

/* Fast popcount for up to 32 bits */
static inline int popcnt32(unsigned int m)
{
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_popcount(m);
#else
    int c = 0;
    while (m) { m &= m - 1; ++c; }
    return c;
#endif
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    if (cache_len < (size_t)n) {
        /* Determine the maximum base‑4 length needed to generate at least n terms */
        const idx_t Lmin = 5;
        idx_t L = Lmin;
        size_t total_terms = 0;
        while (total_terms < (size_t)n) {
            idx_t l_minus = L - 1;
            idx_t comb4 = l_minus * (l_minus - 1) * (l_minus - 2) * (l_minus - 3) / 24;
            size_t pow2 = ((size_t)1) << (L - 5);                 /* L will be ≤ ~32 for normal n */
            size_t contrib = (size_t)l_minus * (size_t)comb4 * pow2;
            total_terms += contrib;
            ++L;
        }
        idx_t Lmax = L - 1;
        size_t capacity = total_terms;

        seq_t *buf = (seq_t*)malloc(capacity * sizeof(seq_t));
        if (!buf) return (seq_t)0;                            /* allocation failure (unlikely) */

        size_t idx_val = 0;

        /* Generate all valid numbers */
        for (idx_t la = Lmin; la <= Lmax; ++la) {
            int Lint = (int)la;
            for (int z = 1; z < Lint; ++z) {                  /* zero cannot be the leading digit */
                int others[64], m = 0;
                for (int p = 0; p < Lint; ++p)
                    if (p != z) others[m++] = p;             /* positions other than z */

                unsigned int rem = m;                        /* = L-1 */
                unsigned int total_masks = 1u << rem;
                for (unsigned int smask = 0; smask < total_masks; ++smask) {
                    if (popcnt32(smask) != 4) continue;      /* need exactly four 3's */

                    int digits[64], rest_idx[64];
                    for (int d = 0; d < Lint; ++d) digits[d] = 1;
                    digits[z] = 0;

                    int rest_cnt = 0;
                    for (int k = 0; k < rem; ++k) {
                        if ((smask >> k) & 1u)
                            digits[ others[k] ] = 3;
                        else
                            rest_idx[rest_cnt++] = others[k];
                    }

                    unsigned int subcnt = 1u << rest_cnt;     /* choices for remaining 1/2 digits */
                    for (unsigned int sub = 0; sub < subcnt; ++sub) {
                        for (int r = 0; r < rest_cnt; ++r)
                            digits[ rest_idx[r] ] = ((sub >> r) & 1u) ? 2 : 1;

                        seq_t val = 0;
                        for (int d = 0; d < Lint; ++d)
                            val = val * 4 + (seq_t)digits[d];
                        buf[idx_val++] = val;
                    }
                }
            }
        }

        /* Sort the entire list */
        qsort(buf, idx_val, sizeof(seq_t), seq_compare);

        /* Replace old cache */
        free(cache);
        cache = buf;
        cache_len = idx_val;
    }

    return cache[n-1];
}
