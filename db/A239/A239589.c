/*
 * A239589  Numbers whose squares are cyclops numbers.
 * https://oeis.org/A239589
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth integer whose square is a cyclops number (odd
 *            number of decimal digits, exactly one zero at the middle, no
 *            other zeros)
 * Range:     u8 n <= 1, u16 n <= 13, u32 n <= 1144, u64 n <= 4000,
 *            u128 n <= 4000, big any n
 * Validated: standard, pass, 62 terms through n = 3348 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static bool is_cyclops_sq(seq_t x)
{
    if (x == 0) return true;         /* 0 is a cyclops number */
    int digits[64];
    int d = 0;
    seq_t tmp = x;
    /* extract digits least‑significant first */
    while (tmp > 0) {
        digits[d++] = (int)(tmp % 10);
        tmp /= 10;
    }
    if (d % 2 == 0) return false;    /* need odd length */
    int mid = d / 2;
    int zero_cnt = 0;
    for (int i = 0; i < d; ++i) {
        if (digits[i] == 0) ++zero_cnt;
    }
    if (zero_cnt != 1) return false;
    return digits[mid] == 0;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static idx_t cache_cap = 0;
    static idx_t next = 0;                 /* start at 0 to include a(1)=0 */

    /* generate terms until we have at least n of them */
    while (cache_len < n) {
        seq_t m = (seq_t)next;
        seq_t sq = m * m;
        if (is_cyclops_sq(sq)) {
            if (cache_len >= cache_cap) {
                idx_t new_cap = cache_cap == 0 ? 8 : cache_cap * 2;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp) exit(1);           /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = m;
        }
        ++next;
    }
    return cache[n-1];
}
