/*
 * A143579  Permutation of the natural numbers (0,1,2,3,...): Odious numbers
 *          (A000069) interleaved with Evil numbers (A001969).
 * https://oeis.org/A143579
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  
   The sequence interleaves odd‑bit‑count (odious) and even‑bit‑count (evil)
   numbers:  
   a(2k)   = odious(k)   (0‑based)  
   a(2k+1) = evil (k)  
   where odious(k) is the k‑th natural number with an odd number of 1‑bits
   in its binary expansion, and evil(k) is defined analogously.  
*/

#include <stdlib.h>

static seq_t *odious_cache   = NULL;
static size_t odious_len     = 0;
static size_t odious_cap     = 0;

static seq_t *evil_cache     = NULL;
static size_t evil_len       = 0;
static size_t evil_cap       = 0;

static seq_t next_candidate = 0;   /* next integer to examine */

/* Allocate an array of seq_t, compatible with both C and C++ */
static seq_t *allocate_seq_arr(size_t n) {
#ifdef __cplusplus
    return new seq_t[n];
#else
    return (seq_t *)malloc(n * sizeof(seq_t));
#endif
}

/* Resize an array, preserving its contents (works in C and C++) */
static void grow_seq_arr(seq_t **arr, size_t *len, size_t *cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t *newp = new seq_t[new_cap];
    for (size_t i = 0; i < *len; ++i) newp[i] = (*arr)[i];
    delete [] *arr;
    *arr = newp;
#else
    seq_t *newp = realloc(*arr, new_cap * sizeof(seq_t));
    *arr = newp;
#endif
    *cap = new_cap;
}

/* Ensure both odious and evil caches contain at least the required
   numbers.  The candidate integer is advanced only once. */
static void ensure_up_to(size_t need_odious, size_t need_evil) {
    while (odious_len <= need_odious || evil_len <= need_evil) {
        /* grow odious cache if needed */
        if (odious_len == odious_cap) {
            size_t new_cap = (odious_cap == 0) ? 16 : odious_cap * 2;
            grow_seq_arr(&odious_cache, &odious_len, &odious_cap, new_cap);
        }
        /* grow evil cache if needed */
        if (evil_len == evil_cap) {
            size_t new_cap = (evil_cap == 0) ? 16 : evil_cap * 2;
            grow_seq_arr(&evil_cache, &evil_len, &evil_cap, new_cap);
        }

        /* evaluate next_candidate */
        seq_t x   = next_candidate;
        seq_t t   = x;
        unsigned int bits = 0;
        while (t) {        /* popcount on seq_t */
            bits += (unsigned int)(t & 1);
            t >>= 1;
        }
        if (bits & 1) {    /* odious */
            odious_cache[odious_len++] = x;
        } else {           /* evil */
            evil_cache[evil_len++] = x;
        }
        ++next_candidate;
    }
}

/* Main sequence generator */
seq_t a(idx_t n) {
    if ((n & 1ULL) == 0) {               /* even index → odious */
        size_t k = (size_t)(n >> 1);
        ensure_up_to(k, 0);
        return odious_cache[k];
    } else {                              /* odd index → evil */
        size_t k = (size_t)(n >> 1);
        ensure_up_to(0, k);
        return evil_cache[k];
    }
}
