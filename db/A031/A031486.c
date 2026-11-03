/*
 * A031486  Numbers whose base-7 representation has 2 fewer 0's than 6's.
 * https://oeis.org/A031486
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 61 terms through n = 955 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A031486: numbers whose base‑7 representation has two fewer
   0's than 6's. */

#include <stddef.h>

#ifdef __cplusplus
#include <vector>
#else
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* C allocation helpers */
static seq_t* seq_alloc_new_array(size_t n)   { return (seq_t*)malloc(n * sizeof(seq_t)); }

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}
#endif

seq_t a(idx_t n)
{
#ifdef __cplusplus
    /* C++ implementation using std::vector for dynamic storage. */
    static std::vector<seq_t> cache;
    static seq_t next = (seq_t)1;   /* next integer to test */

    while (cache.size() < (size_t)n) {
        seq_t x = next;
        unsigned zeros = 0, sixes = 0;

        /* count 0's and 6's in the base‑7 representation */
        while (x > (seq_t)0) {
            uint8_t d = (uint8_t)(x % (seq_t)7);
            if (d == 0)         ++zeros;
            else if (d == 6)    ++sixes;
            x = x / (seq_t)7;
        }

        /* sixes == zeros + 2 */
        if (sixes == zeros + 2)
            cache.push_back(next);

        ++next;
    }

    return cache[(size_t)(n - (idx_t)1)];
#else
    /* C implementation using manually managed array (handles
       simple integer or arbitrary‑precision seq_t). */
    static seq_t *cache = NULL;   /* array of found values       */
    static size_t cap   = 0;      /* allocated capacity          */
    static size_t sz    = 0;      /* number of values stored     */
    static seq_t next   = (seq_t)1; /* next integer to test */

    while (sz < (size_t)n) {
        seq_t x = next;
        unsigned zeros = 0, sixes = 0;

        /* count 0's and 6's in the base‑7 representation */
        while (x > (seq_t)0) {
            uint8_t d = (uint8_t)(x % (seq_t)7);
            if (d == 0)         ++zeros;
            else if (d == 6)    ++sixes;
            x = x / (seq_t)7;
        }

        /* sixes == zeros + 2 */
        if (sixes == zeros + 2) {
            if (sz == cap) {        /* grow the array if needed */
                size_t new_cap = cap ? cap * 2 : 16;
                if (cap == 0)
                    cache = seq_alloc_new_array(new_cap);
                else
                    cache = seq_realloc(cache, cap, new_cap);
                cap = new_cap;
            }
            cache[sz++] = next;
        }
        ++next;
    }

    return cache[(size_t)(n - (idx_t)1)];
#endif
}
