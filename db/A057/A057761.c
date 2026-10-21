/*
 * A057761  Least nonnegative cube root of 2 mod n for n in A057760.
 * https://oeis.org/A057761
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 113, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   find_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A057761: least non‑negative cube root of 2 modulo the
   nth integer that admits such a root.  The sequence is generated
   by iterating over all positive integers, testing whether
   x³ ≡ 2 (mod m).  When a solution exists the smallest
   non‑negative solution is stored.  The function caches the
   results so it can answer calls in any order. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/find_cube_root.h"

static seq_t *root_cache = NULL;   /* cached roots */
static size_t cache_len = 0;       /* number of cached roots */
static size_t cache_cap = 0;       /* allocated capacity of root_cache */
static idx_t next_num = 1;         /* next integer to test (starting at 1) */

/* Ensure capacity of root_cache.  Grow by factor 2. */
static void ensure_cache_cap(void)
{
    if (cache_len >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(root_cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);          /* abort on allocation failure */
        root_cache = tmp;
        cache_cap = new_cap;
    }
}

/* Main function required by the exercise. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* undefined for n==0, safe guard */

    /* Generate roots until we have at least n of them. */
    while (cache_len < n) {
        seq_t r;
        if (find_cube_root(next_num, &r)) {
            ensure_cache_cap();
            root_cache[cache_len++] = r;
        }
        ++next_num;
    }
    return root_cache[n - 1];
}
