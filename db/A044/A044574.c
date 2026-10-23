/*
 * A044574  Numbers k such that string 0,6 occurs in the base 8 representation
 *          of k but not of k+1.
 * https://oeis.org/A044574
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044574: numbers k such that the base‑8 representation of k
 * contains the substring "06" but the base‑8 representation of k+1 does not. */

#include <stdlib.h>
#include <stdbool.h>

#ifdef __cplusplus
#include <vector>
#endif

/* --- Helper: test if a base‑8 representation contains the substring 06 --- */
static bool has06(seq_t v)
{
    if (v == 0)
        return false;
    /* we scan digits from least significant to most; a substring "06"
     * in the original order corresponds to a ...6 then 0 when scanning
     * from least to most significant. */
    int prev_digit = -1;          /* previous (less significant) digit */
    while (v != 0) {
        int d = (int)(v % 8);
        if (d == 0 && prev_digit == 6)
            return true;
        prev_digit = d;
        v /= 8;
    }
    return false;
}

/* --- Shared state for the cache ---------------------------------------- */
static seq_t last_checked = 0;          /* last number examined             */
#ifdef __cplusplus
static std::vector<seq_t> cache_cpp;     /* C++ vector cache                 */
#else
static seq_t *cache_c = NULL;           /* C pointer cache                  */
static size_t  c_cap   = 0;              /* current capacity                  */
static size_t  c_len   = 0;              /* number of elements stored         */
#endif

/* ------------------------------------------------------------------------ */
/* Grow the cache until it contains at least `n` elements.                 */
/* The function is safe to call from any order of `n`.                     */
static void fill_cache(idx_t n)
{
    if (n <= 0)
        return;

#ifdef __cplusplus
    /* C++ implementation – vector grows automatically. */
    while (cache_cpp.size() < n) {
        seq_t k = ++last_checked;
        if (has06(k) && !has06(k + 1))
            cache_cpp.push_back(k);
    }
#else
    /* C implementation – use realloc. */
    while (c_len < n) {
        seq_t k = ++last_checked;
        if (has06(k) && !has06(k + 1)) {
            if (c_len == c_cap) {
                size_t new_cap = (c_cap ? c_cap * 2 : 4);
                seq_t *tmp = (seq_t *)realloc(cache_c, new_cap * sizeof(seq_t));
                if (!tmp) {
                    /* Out of memory – abort for safety. */
                    exit(1);
                }
                cache_c = tmp;
                c_cap  = new_cap;
            }
            cache_c[c_len++] = k;
        }
    }
#endif
}

/* ------------------------------------------------------------------------ */
/* Public API ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                     /* undefined for n < 1 */

    fill_cache(n);

#ifdef __cplusplus
    return cache_cpp[(size_t)(n - 1)];
#else
    return cache_c[(size_t)(n - 1)];
#endif
}
