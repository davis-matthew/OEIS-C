/*
 * A025353  Numbers that are the sum of 3 distinct nonzero squares in 7 or
 *          more ways.
 * https://oeis.org/A025353
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th integer that can be written as the sum of three
 *            * distinct non‑zero squares in at least seven different ways.
 *            The function * constructs the list of such numbers by
 *            enumerating all unordered triples * (a,b,c) with 1 ≤ a < b < c
 *            and counting multiplicities. The cache is * extended u...
 * Range:     u8 none, u16 none, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache_terms   = NULL;   /* sorted list of admissible sums   */
static size_t cache_len     = 0;      /* number of terms stored           */
static int    cache_limit   = 0;      /* sqrt limit used in last build    */

/* free previous cache */
static void free_cache(void) {
    if (cache_terms) {
        free(cache_terms);
        cache_terms = NULL;
    }
    cache_len   = 0;
    cache_limit = 0;
}

/* build terms with squares up to sqrt_limit; store at least need_terms items */
static void build_terms(idx_t sqrt_limit, size_t need_terms) {
    free_cache();                 /* start from scratch for simplicity */

    /* maximal possible sum = 3 * sqrt_limit^2  (fits in idx_t for reasonable limits) */
    idx_t max_sum = (idx_t)3 * sqrt_limit * sqrt_limit;

    /* allocate count array: one slot per possible sum */
    uint16_t *cnt = (uint16_t*)calloc((size_t)(max_sum + 1), sizeof(uint16_t));
    if (!cnt) exit(1);            /* allocation failure – fatal          */

    /* enumerate all unordered triples a<b<c */
    for (idx_t a = 1; a <= sqrt_limit; ++a) {
        seq_t aa = (seq_t)a * a;
        for (idx_t b = a + 1; b <= sqrt_limit; ++b) {
            seq_t ab = aa + (seq_t)b * b;
            for (idx_t c = b + 1; c <= sqrt_limit; ++c) {
                seq_t sum = ab + (seq_t)c * c;
                /* sum fits into max_sum, safely cast to size_t for indexing */
                size_t idx = (size_t)sum;
                if (cnt[idx] < 65535)     /* avoid overflow in the array */
                    ++cnt[idx];
            }
        }
    }

    /* count how many sums have at least 7 representations */
    size_t found = 0;
    for (size_t i = 0; i <= (size_t)max_sum; ++i)
        if (cnt[i] >= 7)
            ++found;

    /* allocate array for the admissible sums */
    seq_t *terms = (seq_t*)malloc(found * sizeof(seq_t));
    if (!terms) { free(cnt); exit(1); }

    /* fill the array in ascending order (the loop over i guarantees this) */
    size_t pos = 0;
    for (size_t i = 0; i <= (size_t)max_sum; ++i)
        if (cnt[i] >= 7)
            terms[pos++] = (seq_t)i;

    free(cnt);

    cache_terms   = terms;
    cache_len     = found;
    cache_limit   = (int)sqrt_limit;
}

/* public interface -------------------------------------------------------- */
seq_t a(idx_t n) {
    if (n == 0) return 0;                        /* out‑of‑range guard     */

    /* ensure cache contains at least n terms */
    if (!cache_terms || cache_len < (size_t)n) {
        idx_t limit = 200;                        /* initial search bound   */
        while (1) {
            build_terms(limit, (size_t)n);
            if (cache_len >= (size_t)n) break;
            /* increase the search radius; exponential growth keeps total work near linear */
            limit *= 2;
        }
    }
    return cache_terms[(size_t)(n - 1)];
}
