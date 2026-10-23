/*
 * A044892  Base-6 run lengths alternate: even, odd, even,...
 * https://oeis.org/A044892
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   numbers whose base-6 run lengths alternate even, odd, even, …,
 *            starting with an even-length run
 * Range:     u8 n <= 30, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function returns the n-th term of the sequence (n ≥ 1).  It keeps
   a static cache of previously computed values so that calls in any
   order remain fast.  The cache is stored as an array of idx_t, which
   is guaranteed to be an unsigned 64‑bit integer (uint64_t).  Only
   integer arithmetic is used; no floating point or external utilities
   are required. */

#include <stdint.h>
#include <stdlib.h>

/* Checks whether x contains an even–odd–even… alternating run of digits
   in base 6, with the first run even.  The routine uses idx_t for all
   arithmetic, so it works for arbitrary large indices.  */
static int passes(idx_t x)
{
    if (x == 0)
        return 0;                      /* no leading zero, single digit fails */

    int digits[64];                   /* base‑6 digits, least significant first */
    int len = 0;
    while (x != 0) {
        digits[len++] = (int)(x % 6);
        x /= 6;
    }

    int run_len = 1;                  /* length of current run */
    int parity_expected = 0;          /* 0 = even, 1 = odd   first run even */

    /* Process from second most significant towards least significant. */
    for (int i = len - 2; i >= 0; --i) {
        if (digits[i] == digits[i + 1]) {          /* same run continues */
            run_len++;
        } else {                                  /* run ends */
            if ((run_len & 1) != parity_expected)
                return 0;                         /* parity mismatch */
            parity_expected ^= 1;                /* switch parity for next run */
            run_len = 1;                          /* start new run */
        }
    }

    /* Check the final run. */
    return ((run_len & 1) == parity_expected);
}

/* Static cache data. */
static idx_t *cache      = NULL;   /* array of already computed terms (as idx_t) */
static size_t cache_cap  = 0;     /* allocated capacity of cache */
static idx_t found        = 0;     /* number of terms cached */
static idx_t next_candidate = 1;  /* next integer to test for membership */

/* Return nth term of the sequence. */
seq_t a(idx_t n)
{
    /* Ensure cache can hold the requested term. */
    if (n > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        while (new_cap < n)
            new_cap *= 2;
        idx_t *tmp = (idx_t *)realloc(cache, new_cap * sizeof(idx_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Extend the cache until we have at least n terms. */
    while (found < n) {
        if (passes(next_candidate)) {
            cache[found] = next_candidate;
            found++;
        }
        next_candidate++;
    }

    /* Convert the cached idx_t value to seq_t and return. */
    return (seq_t)cache[n - 1];
}
