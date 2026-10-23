/*
 * A045011  Numbers whose base-3 representation contains exactly four 0's and
 *          two 2's.
 * https://oeis.org/A045011
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   cmp_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A045011: numbers whose base‑3 representation
   contains exactly four 0's and two 2's.  The routine generates every
   candidate number up to a fixed base‑3 length, sorts them, and returns
   the n‑th smallest element.  This direct enumeration is O(K log K) with
   K ≈ 1.2×10⁴, fast enough for n up to about 10 000. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/cmp_seq_t.h"

static seq_t *cache = NULL;
static idx_t cache_len = 0;
static size_t cache_cap = 0;
static int cache_init = 0;

static inline int popcnt(idx_t x) {
    return __builtin_popcountll((unsigned long long)x);
}

static void init_cache(void) {
    /* Max base‑3 length sufficient to produce more than 10 000 terms */
    const idx_t MAX_LEN = 12;
    const size_t INITIAL_CAP = 20000;
    cache_cap = INITIAL_CAP;
    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    if (!cache) exit(1);

    for (idx_t m = 6; m <= MAX_LEN; ++m) {          /* base‑3 length     */
        const idx_t FULL_MASK = ((idx_t)1 << m) - 1;
        for (idx_t zero_mask = 0; zero_mask <= FULL_MASK; ++zero_mask) {
            if (popcnt(zero_mask) != 4) continue;             /* exactly 4 zeros */
            if ((zero_mask >> (m - 1)) & 1) continue;         /* msb ≠ 0 */

            idx_t rem_mask = FULL_MASK ^ zero_mask;            /* positions free for twos */

            /* enumerate submasks of rem_mask having exactly 2 bits */
            for (idx_t two_mask = 0; two_mask <= rem_mask; ++two_mask) {
                if (popcnt(two_mask) != 2) continue;
                if (two_mask & ~rem_mask) continue;            /* ensure subset */

                seq_t val = 0;
                /* build value from most significant to least significant digit */
                for (idx_t i = m; i-- > 0; ) {
                    unsigned digit = 1;                       /* default digit 1 */
                    if (zero_mask & ((idx_t)1 << i)) digit = 0;
                    else if (two_mask & ((idx_t)1 << i)) digit = 2;
                    val = val * (seq_t)3 + digit;
                }

                /* grow storage if necessary */
                if (cache_len == cache_cap) {
                    cache_cap <<= 1;
                    seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                    if (!tmp) exit(1);
                    cache = tmp;
                }
                cache[cache_len++] = val;
            }
        }
    }

    /* sort the generated numbers */
    qsort(cache, cache_len, sizeof(seq_t), cmp_seq_t);
}

seq_t a(idx_t n) {
    if (!cache_init) {
        init_cache();
        cache_init = 1;
    }
    if (n == 0 || n > cache_len) return 0;   /* undefined for out of bounds */
    return cache[n - 1];
}
