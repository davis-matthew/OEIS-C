/*
 * A043987  Numbers k such that 4 and 6 occur juxtaposed in the base-9
 *          representation of k but not of k+1.
 * https://oeis.org/A043987
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A043987: numbers k such that k's base‑9 representation contains adjacent
   digits 4 and 6 (in either order) but k+1 does not. Computed by direct search. */
#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

/* Return nonzero if n has adjacent digits 4 and 6 (any order) in base‑9 */
static int has46(seq_t n)
{
    seq_t prev = (seq_t)-1;
    while (n > 0) {
        seq_t cur = n % 9;
        if (prev != (seq_t)-1) {
            if ((cur == (seq_t)4 && prev == (seq_t)6) ||
                (cur == (seq_t)6 && prev == (seq_t)4))
                return 1;
        }
        prev = cur;
        n /= 9;
    }
    return 0;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
#ifdef __cplusplus
    static std::vector<seq_t> cache;
    static seq_t cur = 0;
    while (cache.size() < n) {
        ++cur;
        if (has46(cur) && !has46(cur + (seq_t)1))
            cache.push_back(cur);
    }
    return cache[n - 1];
#else
    static seq_t *cache = NULL;
    static size_t len = 0, cap = 0;
    static seq_t cur = 0;
    while (len < n) {
        ++cur;
        if (has46(cur) && !has46(cur + (seq_t)1)) {
            if (len >= cap) {
                size_t new_cap = cap ? cap * 2 : 64;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) return 0;  /* allocation failure */
                cache = tmp;
                cap = new_cap;
            }
            cache[len++] = cur;
        }
    }
    return cache[n - 1];
#endif
}
