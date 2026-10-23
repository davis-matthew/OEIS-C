/*
 * A031560  Numbers k such that the continued fraction for sqrt(k) has even
 *          period and if the last term of the periodic part is deleted the
 *          central term is 62.
 * https://oeis.org/A031560
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth integer k such that the continued fraction
 *            expansion of √k has even period and after deleting the last term
 *            of that period the central term of the remaining sequence equals
 *            62. (Computed by brute‑force search.)
 * Range:     u8 none, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/floor_sqrt.h"

static int is_target(seq_t k)
{
    if (k < 2) return 0;
    seq_t a0 = floor_sqrt(k);
    if (a0 * a0 == k) return 0;              /* perfect square */
    seq_t m = 0, d = 1, a = a0;

    /* The period length for our search is well below 4000, so this buffer is safe. */
    static seq_t period[4096];
    int len = 0;

    while (1) {
        m = d * a - m;
        d = (k - m * m) / d;
        a = (a0 + m) / d;
        period[len++] = a;
        if (a == a0 * 2) break;          /* end of the period */
    }

    if (len % 2) return 0;                /* period must be even */
    int mid = (len - 1) / 2;
    return period[mid] == (seq_t)62 ? 1 : 0;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined, return 0 for safety */

    static seq_t *cache = NULL;
    static size_t sz = 0, cap = 0;
    static seq_t next = 2;                 /* next candidate to test */

    while (sz < (size_t)n) {
        while (!is_target(next)) {
            ++next;
        }

        if (sz >= cap) {
            size_t newcap = cap ? cap * 2 : 64;
            while (newcap <= sz + 1) newcap <<= 1;
            seq_t *tmp = (seq_t *)malloc(newcap * sizeof(seq_t));
            if (cache) {
                memcpy(tmp, cache, sz * sizeof(seq_t));
                free(cache);
            }
            cache = tmp;
            cap = newcap;
        }

        cache[sz++] = next;
        ++next;
    }

    return cache[n - 1];
}
