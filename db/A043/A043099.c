/*
 * A043099  Numbers k such that 1 and 2 occur juxtaposed in the base-3
 *          representation of k but not of k-1.
 * https://oeis.org/A043099
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth integer k whose base‑3 representation contains
 *            the digits 1 and 2 adjacent (either 12 or 21) while (k‑1)'s
 *            base‑3 representation does not. Indexing starts at 1
 * Range:     u8 n <= 34, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Determine whether the base‑3 representation of x contains the
   digits 1 and 2 adjacent in either order. */
static bool has_adj12(idx_t x)
{
    if (x == 0) return false;          /* base‑3 0 has no adjacency */
    unsigned char digits[32];
    int len = 0;
    idx_t y = x;
    while (y > 0) {
        digits[len++] = y % 3;          /* least‑significant first */
        y /= 3;
    }
    /* check from most significant to least */
    for (int i = len - 1; i > 0; --i) {
        unsigned char d1 = digits[i];
        unsigned char d0 = digits[i - 1];
        if ((d1 == 1 && d0 == 2) || (d1 == 2 && d0 == 1))
            return true;
    }
    return false;
}

/* Cache of already computed terms.  All objects are static so that
   repeated calls can reuse the data. */
static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t cache_cap = 0;
static idx_t next_k = 1;                /* next integer to test */

static void ensure_cache_capacity(idx_t needed)
{
    if (cache_cap < needed) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 32;
        while (new_cap < needed) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);              /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Public function returning the nth term of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* offset is 1, undefined for 0 */

    /* generate terms until we have at least n of them */
    while (cache_len < n) {
        if (has_adj12(next_k) && !has_adj12(next_k - 1)) {
            ensure_cache_capacity(cache_len + 1);
            cache[cache_len++] = (seq_t)next_k;
        }
        ++next_k;
    }
    return cache[n - 1];
}
