/*
 * A102837  Numbers with property that can bring the first digit to the back
 *          of the number to get a square (zeros are dropped).
 * https://oeis.org/A102837
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 49, u32 n <= 614, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digits_seq, is_square, pow10_seq, resize_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A102837:  
   a(n) = smallest integer x such that moving the first digit of x to
   the back (zeros dropped) yields a perfect square.  The terms are
   generated in increasing order of x.  The implementation caches
   previously computed terms so that subsequent calls with arbitrary
   n are fast. */

#include <stdbool.h>
#include "../utils/digits_seq.h"
#include "../utils/pow10_seq.h"
#include "../utils/is_square.h"
#include "../utils/resize_array.h"

/* Helper: test whether a number satisfies the property */
static bool good(seq_t x) {
    /* number of decimal digits of x */
    idx_t d = digits_seq(x);
    /* 10^(d-1) */
    seq_t pow = pow10_seq(d - 1);
    /* most significant digit */
    seq_t msd = x / pow;
    /* remaining part */
    seq_t rest = x % pow;
    /* number after moving first digit to the back */
    seq_t y = rest * 10 + msd;
    return is_square(y);
}

/* Cached sequence data */
static seq_t *cache = NULL;       /* array of terms */
static idx_t cache_len = 0;       /* number of stored terms */
static idx_t cache_cap = 0;       /* allocated capacity of cache */

/* Current integer being tested */
static seq_t cur = 1;

/* Return n-th term of the sequence (1‑based indexing) */
seq_t a(idx_t n) {
    if (n == 0) { /* undefined for n<1, but return 0 to avoid UB */
        return 0;
    }

    /* Generate terms until we have at least n of them */
    while (cache_len < n) {
        if (good(cur)) {
            /* Grow cache if necessary */
            if (cache_len == cache_cap) {
                idx_t new_cap = cache_cap == 0 ? 8 : cache_cap * 2;
                cache = resize_array(cache, cache_cap, new_cap);
                cache_cap = new_cap;
            }
            cache[cache_len++] = cur;
        }
        cur = cur + 1;  /* proceed to next integer */
    }

    return cache[n - 1];
}
