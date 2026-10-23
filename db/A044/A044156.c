/*
 * A044156  Numbers n such that string 2,4 occurs in the base 7 representation
 *          of n but not of n-1.
 * https://oeis.org/A044156
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

/* Sequence A044156
   a(n) = the n‑th positive integer whose base‑7 representation contains
   the consecutive digits "24" but the previous integer does not.
   Offset = 1 (a(1)=18).
   This implementation searches integers in increasing order and
   lazily caches results.  It is O(N) in the index n for the already
   generated part and works for any index order. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Return true iff the base‑7 representation of x contains the substring
   "24" (digit 2 followed immediately by digit 4).  */
static bool has24(idx_t x)
{
    if (x == 0) return false;          /* base‑7 "0" never contains "24" */
    idx_t prev = x % 7;                /* least significant digit */
    x /= 7;
    while (x != 0) {
        idx_t d = x % 7;               /* next more significant digit */
        if (d == 2 && prev == 4) return true;
        prev = d;
        x /= 7;
    }
    return false;
}

/* Reallocate an array of seq_t values, with a cast that works both in
   C and C++.  The prefix __cplusplus is defined when compiling as
   C++.  */
static seq_t* realloc_cache(seq_t *p, size_t new_cap)
{
#if defined(__cplusplus)
    return reinterpret_cast<seq_t*>(realloc(p, new_cap * sizeof(seq_t)));
#else
    return (seq_t*)realloc(p, new_cap * sizeof(seq_t));
#endif
}

/* Main sequence function.  */
seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for index 0 (not requested) */
        return (seq_t)0;

    static idx_t cur = 1;      /* integer being examined */
    static seq_t *cache = NULL;/* cached sequence values */
    static size_t cap = 0;     /* size of allocated cache */
    static size_t sz  = 0;     /* number of values stored */

    while (sz < n) {
        bool cur_has  = has24(cur);
        bool prev_has = has24(cur - 1);

        if (cur_has && !prev_has) {
            if (sz == cap) {
                cap = cap ? cap * 2 : 64;
                seq_t *tmp = realloc_cache(cache, cap);
                if (!tmp) {
                    free(cache);
                    abort();
                }
                cache = tmp;
            }
            cache[sz++] = (seq_t)cur;
        }
        cur++;
    }
    return cache[n - 1];
}
