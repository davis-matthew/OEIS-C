/*
 * A092574  Positive integers that can be represented in the form x^2 + 3y^2
 *          with (x,y) = 1 and x and y positive.
 * https://oeis.org/A092574
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: sieve all primitive representations x^2 + 3y^2 with gcd(x,y)=1
   (x>0,y>0).  Collect the resulting integers in increasing order into a
   cache.  The function a(n) returns the n‑th term from that cache, rebuilding
   the cache with a larger bound if necessary. */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

/* integer square root for unsigned 64‑bit values */
static idx_t isqrt(idx_t n) {
    idx_t r = (idx_t) sqrt((long double)n);
    while ((r + 1) * (r + 1) <= n) ++r;
    while (r * r > n) --r;
    return r;
}

/* greatest common divisor for idx_t values */
static idx_t gcd_idx(idx_t a, idx_t b) {
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* cache of sequence values and current size */
static seq_t *seq_cache = NULL;   /* array of terms a(1),a(2),... */
static idx_t seq_len   = 0;       /* number of terms stored in seq_cache */

/* largest bound that has been sieved; used to decide when to extend the cache */
static idx_t current_limit = 0;

/* rebuild the cache so that it contains at least N terms */
static void build_sequence(idx_t N) {
    /* choose an initial bound; grow exponentially until we have enough terms */
    idx_t limit = 200000;           /* start with a reasonable size */
    if (current_limit > 0 && current_limit > N) {
        limit = current_limit * 2;  /* start from a larger bound if already sieved */
    }

    while (1) {
        /* allocate visited array for numbers up to 'limit' */
        unsigned char *visited = (unsigned char *)calloc((size_t)(limit + 1), 1);
        if (!visited) abort();     /* allocation failed */

        /* count unique representable numbers within the limit */
        idx_t count = 0;
        idx_t x_max = isqrt(limit);
        for (idx_t x = 1; x <= x_max; ++x) {
            idx_t x2 = x * x;
            if (x2 + (idx_t)3 > limit) continue;
            idx_t y_max = isqrt((limit - x2) / 3);
            for (idx_t y = 1; y <= y_max; ++y) {
                if (gcd_idx(x, y) != 1) continue;
                seq_t n_val = (seq_t)x2 + (seq_t)3 * (seq_t)y * (seq_t)y;
                if (n_val > (seq_t)limit) continue;
                idx_t idx = (idx_t)n_val;
                if (!visited[idx]) {
                    visited[idx] = 1;
                    ++count;
                }
            }
        }

        /* collect the values into a temporary array */
        seq_t *new_cache = (seq_t *)malloc((size_t)count * sizeof(seq_t));
        if (!new_cache) abort();
        idx_t pos = 0;
        for (idx_t n = 1; n <= limit; ++n) {
            if (visited[n]) {
                new_cache[pos++] = (seq_t)n;
            }
        }
        free(visited);

        /* replace the old cache (if any) with the new one */
        free(seq_cache);
        seq_cache = new_cache;
        seq_len   = count;
        current_limit = limit;

        if (seq_len >= N) return;          /* we have enough terms */
        limit *= 2;                         /* not enough, enlarge the bound */
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0;                 /* out of range handling */
    if (seq_len < n) build_sequence(n);   /* ensure table has at least n terms */
    return seq_cache[n - 1];              /* zero‑based indexing */
}
