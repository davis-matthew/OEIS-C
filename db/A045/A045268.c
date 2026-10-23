/*
 * A045268  Numbers whose base-5 representation contains exactly one 2 and two
 *          3's.
 * https://oeis.org/A045268
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose base‑5 representation contains exactly one digit 2
   and two digits 3.  */
#include <stdbool.h>
#include <stddef.h>
#ifdef __cplusplus
#  include <cstddef>
#endif

/* ----------------------------------------------------------------------
   Portable allocation for seq_t, handled differently in C++ (new/delete)
   and in C (malloc/free).  The build supplies a definition for seq_t.
   ---------------------------------------------------------------------- */
#ifdef __cplusplus
#  define SEQ_ALLOC(n)   new seq_t[(n)]
#  define SEQ_FREE(p)    delete[] (p)
#else
#  include <stdlib.h>
#  define SEQ_ALLOC(n)   (seq_t*)malloc(sizeof(seq_t)*(n))
#  define SEQ_FREE(p)    free(p)
#endif

/* ----------------------------------------------------------------------
   Check whether a number has exactly one 2 and exactly two 3's in base‑5.
   ---------------------------------------------------------------------- */
static inline bool matches(seq_t v)
{
    unsigned count2 = 0, count3 = 0;
    while (v != 0) {
        seq_t d = v % 5;
        v /= 5;
        if (d == 2) {
            if (++count2 > 1) return false;
        } else if (d == 3) {
            if (++count3 > 2) return false;
        }
    }
    return (count2 == 1 && count3 == 2);
}

/* ----------------------------------------------------------------------
   Cache for generated terms and the next value to test.
   ---------------------------------------------------------------------- */
static seq_t *cache      = NULL;  /* array of found terms */
static size_t cache_sz   = 0;     /* current number of terms stored */
static size_t cache_cap  = 0;     /* array capacity */
static seq_t next_val    = 1;     /* value to test next */

/* ----------------------------------------------------------------------
   Grow the cache array to accommodate at least 'needed' elements.
   ---------------------------------------------------------------------- */
static inline void cache_grow(size_t needed)
{
    if (cache_cap >= needed) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 64;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *new_ptr = SEQ_ALLOC(new_cap);
    for (size_t i = 0; i < cache_sz; ++i) new_ptr[i] = cache[i];
    SEQ_FREE(cache);
    cache     = new_ptr;
    cache_cap = new_cap;
}

/* ----------------------------------------------------------------------
   Main sequence function.
   ---------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n = 0 */

    size_t target = (size_t)n;
    while (cache_sz < target) {
        if (matches(next_val)) {
            cache_grow(cache_sz + 1);
            cache[cache_sz++] = next_val;
        }
        ++next_val;
    }
    return cache[target - 1];
}
