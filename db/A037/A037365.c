/*
 * A037365  Numbers whose base-6 and base-9 expansions have no digits in
 *          common.
 * https://oeis.org/A037365
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th positive integer whose base‑6 and base‑9
 *            representations have no digits in common. Computed by scanning
 *            successive integers and caching results
 * Range:     u8 n <= 76, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* helper: test whether a number has no common digits in base 6 and base 9 */
static bool qualifies(seq_t n) {
    unsigned int mask6 = 0, mask9 = 0;
    seq_t t = n;
    do {                 /* base‑6 digits */
        unsigned int d = (unsigned int)(t % 6);
        mask6 |= 1u << d;
        t /= 6;
    } while (t > 0);
    t = n;
    do {                 /* base‑9 digits */
        unsigned int d = (unsigned int)(t % 9);
        mask9 |= 1u << d;
        t /= 9;
    } while (t > 0);
    return (mask6 & mask9) == 0;
}

/* main sequence function */
seq_t a(idx_t n) {
    /* empty or negative n is out of definition range */
    if (n == 0) return 0;

    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t next_candidate = 1; /* next integer to test */

    /* grow cache until it contains at least n elements */
    while (cache_len < n) {
        seq_t cand = next_candidate;
        ++next_candidate;         /* advance for next round */

        if (!qualifies(cand))
            continue;             /* candidate rejected */

        /* extend cache if needed */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap == 0 ? 128 : cache_cap << 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0; /* allocation failure, return 0 */
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[cache_len++] = cand;
    }
    return cache[n - 1];
}
