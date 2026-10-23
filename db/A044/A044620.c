/*
 * A044620  Numbers n such that string 6,4 occurs in the base 8 representation
 *          of n but not of n+1.
 * https://oeis.org/A044620
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   get_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A044620 (Numbers n such that the substring 6 4 occurs in the base‑8 representation of n but not in n+1).
   The implementation performs an on‑demand linear search with memoisation.
   For each integer candidate it tests the pattern by iterating over the base‑8 digits.
*/

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/get_cache.h"

static bool has_pattern(seq_t x) {
    // Return true if the base‑8 representation of x contains the substring 6,4
    bool prev_valid = false;
    int prev = 0;
    while (x != 0) {
        int cur = (int)(x % 8);
        if (prev_valid && cur == 6 && prev == 4)
            return true;
        prev_valid = true;
        prev = cur;
        x /= 8;
    }
    return false;
}

static void ensure_capacity(size_t needed) {
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static size_t sz = 0;   // number of elements stored
    static idx_t last_tested = 0;  // last integer checked

    // This function is called only from a(), so we expose cache and other
    // variables via a couple of 'static' pointers for use outside, see a().
    (void)needed; /* dummy, actual logic in a() retrieves/sets static vars */

    /* The outer function a() will manage the cache and the last_tested
       variable; this helper only resizes the array when needed.   */
    /* To keep this helper self‑contained, we do nothing here; the
       a() function will use the same integrity.  */
}

/* Main sequence function */
seq_t a(idx_t n) {
    if (n == 0) return 0;  /* not used, but safe */

    /* Static state for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* number of elements stored */
    static size_t cache_cap = 0;    /* allocated capacity */
    static idx_t last_tested = 0;   /* last integer inspected */

    if (n <= cache_size)
        return cache[n - 1];

    /* Expand cache until we have at least n elements */
    for (; cache_size < (size_t)n; ++last_tested) {
        idx_t cand = last_tested + 1;
        bool contains = has_pattern((seq_t)cand);
        if (contains && !has_pattern((seq_t)(cand + 1))) {
            if (cache_size == cache_cap) {
                size_t newcap = cache_cap ? cache_cap * 2 : 8;
                seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
                if (!tmp) abort();  /* allocation failure */
                cache = tmp;
                cache_cap = newcap;
            }
            cache[cache_size++] = (seq_t)cand;
        }
    }

    return cache[n - 1];
}
