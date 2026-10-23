/*
 * A047150  Array read by diagonals: T(h,k)=number of paths consisting of
 *          steps from (0,0) to (h,k) such that each step has length 1
 *          directed up or right and no up-step crosses the line y=5x/3. (Thus
 *          a path crosses the line only at lattice points and on
 *          right-steps.).
 * https://oeis.org/A047150
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A047150: array read by diagonals – T(h,k)=#paths from (0,0) to (h,k)
   with steps up or right and an up‑step never crossing y=5x/3
   (crossings allowed only at lattice points and on right‑steps).
   a(n)=T(h,k) where h+k=n.  Recurrence:
      T(0,0)=1
      T(h,k)=T(h-1,k) + (k>0 && up_allowed(h,k-1))? T(h,k-1) : 0  (h>0)
   where up_allowed(h,y) returns true iff the vertical step from
   (h,y) to (h,y+1) does not intersect the line in its interior. */
#include <stdbool.h>
#include <stdlib.h>

/* Return true iff the up‑step from (h,y) to (h,y+1) does not cross the line y=5x/3.
   The step is forbidden only when the line passes strictly inside the segment,
   i.e., when y == floor(5h/3) and 5h is not divisible by 3. */
static inline bool up_allowed(idx_t h, idx_t y)
{
    idx_t r = (5 * h) % 3;
    if (r == 0)            /* line visits a lattice point at x = h */
        return true;
    /* Check whether y equals floor(5h/3).  If yes, the step is forbidden. */
    return (3 * y) != (5 * h - r);
}

seq_t a(idx_t n)
{
    /* Static cache for computed a(n) values (triangular enumeration). */
    static seq_t *cache  = NULL;   /* values in order of a(0), a(1), ... */
    static size_t cache_cap = 0;   /* allocated capacity of cache */
    static idx_t cache_len  = 0;   /* number of cached terms */

    /* Buffers for the most recent two diagonals. */
    static seq_t *diag_prev = NULL; /* values for diagonal d-1  (size = d) */
    static size_t diag_prev_len = 0;
    static idx_t current_diag = 0;  /* d of the last computed diagonal */

    /* First call: initialise cache and diagonal 0. */
    if (cache_len == 0) {
        cache_cap = 128;
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        cache[0] = (seq_t)1;      /* a(0) = T(0,0) */
        cache_len = 1;

        diag_prev_len = 1;
        diag_prev = (seq_t *)calloc(diag_prev_len, sizeof(seq_t));
        diag_prev[0] = (seq_t)1;  /* T(0,0) */
        current_diag = 0;
    }

    /* Grow cache until it contains a(n). */
    while (cache_len <= n) {
        idx_t d = current_diag + 1;          /* next diagonal index */
        size_t new_len = d + 1;              /* cells on this diagonal, h=0..d */

        seq_t *diag_cur = (seq_t *)calloc(new_len, sizeof(seq_t));

        for (idx_t h = 0; h < new_len; ++h) {
            idx_t k = d - h;                 /* k = d-h so that h+k=d */
            seq_t val = 0;

            /* Contribution from a right step from (h-1,k). */
            if (h > 0)
                val += diag_prev[h - 1];

            /* Contribution from an up step from (h,k-1). */
            if (k > 0 && up_allowed(h, k - 1))
                val += diag_prev[h];

            diag_cur[h] = val;

            /* Append the value to the global cache. */
            if (cache_len == cache_cap) {
                cache_cap <<= 1;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = val;
        }

        free(diag_prev);
        diag_prev     = diag_cur;
        diag_prev_len = new_len;
        current_diag  = d;
    }

    return cache[n];
}
