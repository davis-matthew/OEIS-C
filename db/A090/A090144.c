/*
 * A090144  Numbers n which when converted to some base between 2 and 9 yield
 *          a result with the same digits as n in a different order.
 * https://oeis.org/A090144
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A090144
 * a(n) = the n-th positive integer that, for some base b (2 ≤ b ≤ 9),
 * has its representation in base b a permutation of its decimal digits.
 * The function generates terms incrementally and caches them for reuse.
 */
#include <stdlib.h>
#include <stdbool.h>

/* Provided by the build environment */
seq_t a(idx_t n)
{
    /* Cache of found terms; cache[1] contains a(1). */
    static seq_t *cache = NULL;  /* pointer to array of seq_t */
    static size_t cap = 0;       /* current capacity of cache */
    static size_t sz  = 0;       /* number of stored terms (including sentinel at 0) */
    static seq_t current = 0;    /* last checked candidate */

    /* Initialise cache on first call. */
    if (!cache) {
        cap = 16;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!cache) exit(1);
        cache[0] = 0;            /* sentinel, not used for a(n) */
        sz = 1;
        current = 0;
    }

    /* Generate terms until we have at least n of them. */
    while (sz <= (size_t)n) {
        seq_t cand = current + (seq_t)1;   /* candidate integer to test */

        /* Compute digit frequencies of cand in base 10. */
        int dec[10] = {0};
        seq_t t = cand;
        if (t == 0) {
            dec[0] = 1;
        } else {
            while (t != 0) {
                int d = (int)(t % (seq_t)10);
                dec[d] += 1;
                t /= (seq_t)10;
            }
        }

        bool ok = false;
        /* Check all bases 2 through 9. */
        for (int base = 2; base <= 9 && !ok; ++base) {
            int cnt[10] = {0};
            seq_t u = cand;
            if (u == 0) {
                cnt[0] = 1;
            } else {
                while (u != 0) {
                    int d = (int)(u % (seq_t)base);
                    cnt[d] += 1;
                    u /= (seq_t)base;
                }
            }
            /* Compare frequency tables. */
            bool same = true;
            for (int i = 0; i < 10; ++i) {
                if (dec[i] != cnt[i]) {
                    same = false;
                    break;
                }
            }
            if (same) ok = true;
        }

        if (ok) {
            /* Ensure capacity. */
            if (sz == cap) {
                size_t newcap = cap * 2;
                seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
                if (!tmp) exit(1);
                cache = tmp;
                cap = newcap;
            }
            cache[sz++] = cand;
        }

        current = cand;
    }

    return cache[sz - 1];
}
