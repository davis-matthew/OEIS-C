/*
 * A025407  Numbers that are the sum of 4 positive cubes in 3 or more ways.
 * https://oeis.org/A025407
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: precompute all sums of four positive cubes ≤200
   and collect those that have at least three distinct unordered representations. */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static seq_t *terms = NULL;
static idx_t term_count = 0;

static void init_terms(void)
{
    const idx_t m = 200;                         /* upper bound for cube roots */
    uint32_t cube[201];
    for (idx_t i = 0; i <= m; ++i)               /* precompute cubes */
        cube[i] = (uint32_t)(i * i * i);

    idx_t max_sum = 4 * cube[m];                 /* 4 * 200^3 = 32,000,000 */
    size_t array_size = (size_t)max_sum + 1;
    uint16_t *cnt = (uint16_t *)malloc(array_size * sizeof(uint16_t));
    if (!cnt) return;                            /* allocation failed, leave empty */

    memset(cnt, 0, array_size * sizeof(uint16_t));
    for (idx_t a = 1; a <= m; ++a) {
        for (idx_t b = a; b <= m; ++b) {
            uint32_t ab = cube[a] + cube[b];
            for (idx_t c = b; c <= m; ++c) {
                uint32_t abc = ab + cube[c];
                for (idx_t d = c; d <= m; ++d) {
                    uint32_t sum = abc + cube[d];
                    ++cnt[sum];
                }
            }
        }
    }

    idx_t count_terms = 0;                       /* first pass: count how many meet the threshold */
    for (size_t s = 1; s < array_size; ++s)
        if (cnt[s] >= 3)
            ++count_terms;

    seq_t *tmp = (seq_t *)malloc(count_terms * sizeof(seq_t));
    if (!tmp) {
        free(cnt);
        return;
    }

    idx_t pos = 0;                               /* second pass: collect the terms in order */
    for (size_t s = 1; s < array_size; ++s)
        if (cnt[s] >= 3)
            tmp[pos++] = (seq_t)s;

    free(cnt);
    terms = tmp;
    term_count = count_terms;
}

seq_t a(idx_t n)
{
    if (terms == NULL)
        init_terms();

    if (n == 0 || n > term_count)
        return (seq_t)0;                         /* out of range: return 0 (undefined for OEIS offset) */

    return terms[n - 1];
}
