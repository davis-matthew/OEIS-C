/*
 * A044781  Numbers n such that string 6,8 occurs in the base 10
 *          representation of n but not of n+1.
 * https://oeis.org/A044781
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers n such that the decimal representation of n contains
 * the substring "68" but that of n+1 does not.  The function searches
 * sequentially for such integers and memoises the results */
#include <stdlib.h>
#include <stdbool.h>

#ifdef __cplusplus
#include <vector>
#endif

/* Return true iff the decimal representation of x contains the substring
 * "68".  Works for both ordinary unsigned integers and an arbitrary‑
 * precision type provided by the build system. */
static bool contains68(seq_t x)
{
    int prev = -1;                          /* previous decimal digit */
    while (x > 0) {
        int d = (int)(x % 10);              /* current least‑significant digit */
        if (prev == 8 && d == 6) return true;
        prev = d;
        x /= 10;
    }
    return false;
}

/* Return the n‑th term of the sequence; a(1) is the first listed term.
 * The sequence is memoised so that calls in any order remain efficient. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;    /* undefined for the offset, harmless */

#ifdef __cplusplus
    /* C++ implementation – use std::vector for dynamic array */
    static std::vector<seq_t> cache;            /* stored terms */
    static seq_t last_checked = 0;             /* last candidate tested */

    while (cache.size() < n) {
        seq_t cand = ++last_checked;
        if (contains68(cand) && !contains68(cand + 1))
            cache.push_back(cand);
    }
    return cache[(size_t)(n - 1)];
#else
    /* C implementation – use malloc/realloc for dynamic array */
    static seq_t *cache = NULL;          /* array of found terms */
    static idx_t size = 0;               /* number of cached terms   */
    static idx_t cap  = 0;               /* allocated capacity       */
    static seq_t last_checked = 0;       /* last candidate tested    */

    while (size < n) {
        seq_t cand = ++last_checked;
        if (contains68(cand) && !contains68(cand + 1)) {
            if (size == cap) {
                idx_t new_cap = cap ? cap * 2 : 16;
                seq_t *tmp = realloc(cache, (size_t)new_cap * sizeof(seq_t));
                if (!tmp) exit(EXIT_FAILURE);
                cache = tmp;
                cap = new_cap;
            }
            cache[size++] = cand;
        }
    }
    return cache[(size_t)(n - 1)];
#endif
}
