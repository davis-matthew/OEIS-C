/*
 * A124276  Terms k of A068563 such that k/2 is not a term of A068563.
 * https://oeis.org/A124276
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 376, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A124276: terms k of A068563 such that k/2 is not a term of A068563. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static seq_t *cache = NULL;          /* array of terms */
static idx_t cache_len = 0;          /* current number of terms cached   */
static idx_t cache_cap = 0;          /* allocated capacity of cache      */
static idx_t checked   = 0;          /* last integer examined (exclusive) */

/* fast modular exponentiation: base^exp mod mod_val (uint64_t). */
static uint64_t mod_pow_uint64(uint64_t base, idx_t exp, uint64_t mod_val) {
    uint64_t res = 1 % mod_val;
    uint64_t cur = base % mod_val;
    while (exp > 0) {
        if (exp & 1U)
            res = (unsigned __int128)res * cur % mod_val;
        cur = (unsigned __int128)cur * cur % mod_val;
        exp >>= 1;
    }
    return res;
}

/* test whether n is in A068563 */
static bool is_A068563(idx_t n) {
    if (n == 0) return false;          /* 0 is not considered */
    uint64_t m = n;
    while ((m & 1ULL) == 0)            /* strip powers of two */
        m >>= 1;
    if (m == 1)                         /* powers of two are always in A068563 */
        return true;
    return mod_pow_uint64(2, n, m) == 1;
}

/* enlarge the cache if it cannot hold at least 'need' elements */
static void grow_cache(idx_t need) {
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < (size_t)need) new_cap <<= 1;
    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = new_ptr;
    cache_cap = new_cap;
}

/* return the n‑th term of A124276 (1‑based).  The sequence is generated on
 * demand and cached for future calls. */
seq_t a(idx_t n) {
    if (n == 0) return 0;              /* undefined for 0, return 0 */

    if (cache_len < n) {
        grow_cache(n);
        idx_t cand = checked + 1;      /* start from the first integer not yet examined */
        while (cache_len < n) {
            if (is_A068563(cand)) {
                /* include if odd or its half is not in A068563 */
                if ((cand & 1ULL) || !is_A068563(cand >> 1)) {
                    cache[cache_len++] = (seq_t)cand;
                }
            }
            ++cand;
        }
        checked = cand - 1;            /* last examined integer */
    }
    return cache[n - 1];               /* 1‑based indexing */
}
