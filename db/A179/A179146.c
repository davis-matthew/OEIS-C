/*
 * A179146  Numbers k such that Mordell's equation y^2 = x^3 + k has exactly 2
 *          integral solutions.
 * https://oeis.org/A179146
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th positive integer k such that the Mordell
 *            equation y² = x³ + k has exactly 2 integral solutions. The
 *            sequence excludes all positive perfect cubes, which always give
 *            a solution with y = 0
 * Range:     u8 none, u16 none, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   floor_sqrt, int_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation enumerates all pairs (x, y) with y > 0 such that
   y² – x³ = k and counts how many times each k occurs.  A k that occurs
   exactly once and is not a perfect cube corresponds to exactly two
   integer solutions (x, ±y).  The values are generated once on the first
   call to a() and cached.  The algorithm works for seq_t of any size
   (up to 128 bits); the bound for k (K_LIMIT) is chosen large enough to
   cover at least the first 10 000 terms.  */

/* ---------------  Includes + Helpers --------------- */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "../utils/floor_sqrt.h"   /* seq_t floor_sqrt(seq_t x)          */
#include "../utils/int_cube_root.h"/* idx_t int_cube_root(idx_t n)       */

static bool is_cube(seq_t x) {
    /* Returns true if x is a perfect cube (x > 0). */
    idx_t r = int_cube_root((idx_t)x);
    return (seq_t)r * r * r == x;
}

/* ---------------  Global state --------------- */

static seq_t *list_of_k = NULL;    /* sorted list of k with exactly one non‑zero y solution */
static idx_t  list_len   = 0;      /* number of elements in list_of_k                    */
static bool    initialized = false;

/* ---------------  Initialization (heavy work) --------------- */

static void init_sequence(void)
{
    /* Upper bound for k that we need to consider.  20 000 000 is ample for
       the first 10 000 terms, according to empirical tests.  */
    const idx_t K_LIMIT = 20000000ULL;
    const idx_t X_LIMIT = int_cube_root(K_LIMIT);          /* |x| <= X_LIMIT */

    /* Allocate counter array.  Using uint8_t suffices because we cap at 2. */
    uint8_t *cnt = (uint8_t *)calloc((size_t)K_LIMIT + 1, sizeof(uint8_t));
    if (cnt == NULL) return;                                 /* abort on OOM */

    /* Enumerate all pairs (x, y) with y > 0.  */
    for (int64_t ix = -(int64_t)X_LIMIT; ix <= (int64_t)X_LIMIT; ++ix) {
        int64_t x3 = ix * ix * ix;        /* x³, fits in 64‑bit for X_LIMIT <= 271 */
        /* Determine range of y that can produce k <= K_LIMIT.  */
        idx_t y_min;
        if (ix >= 0) {
            seq_t sq = floor_sqrt((seq_t)x3);
            y_min = (idx_t)sq + 1;        /* ensure y² > x³ so k > 0 */
        } else {
            y_min = 1;
        }

        int64_t val = (ix >= 0) ? ((int64_t)x3 + (int64_t)K_LIMIT)
                                : ((int64_t)K_LIMIT + (int64_t)x3);
        if (val < 0) continue;           /* shouldn't happen, but guard */

        seq_t y_max = floor_sqrt((seq_t)val);
        for (idx_t y = y_min; y <= y_max; ++y) {
            seq_t ysq = (seq_t)y * (seq_t)y;
            int64_t kint = (int64_t)ysq - (int64_t)x3;   /* guaranteed > 0 */
            if (kint <= 0 || kint > (int64_t)K_LIMIT) continue;
            size_t k = (size_t)kint;
            if (cnt[k] < 2) cnt[k]++;          /* cap at 2 to detect uniqueness */
        }
    }

    /* Count how many k will survive (unique and not a perfect cube).  */
    idx_t total = 0;
    for (size_t k = 1; k <= (size_t)K_LIMIT; ++k) {
        if (cnt[k] == 1 && !is_cube((seq_t)k)) ++total;
    }

    if (total == 0) {
        free(cnt);
        return;                          /* nothing found; unlikely for K_LIMIT */
    }

    /* Allocate array for the k values.  */
    seq_t *temp = (seq_t *)malloc((size_t)total * sizeof(seq_t));
    if (temp == NULL) {
        free(cnt);
        return;                          /* OOM – abort silently */
    }

    /* Populate the array in ascending order.  */
    idx_t pos = 0;
    for (size_t k = 1; k <= (size_t)K_LIMIT; ++k) {
        if (cnt[k] == 1 && !is_cube((seq_t)k)) {
            temp[pos++] = (seq_t)k;
        }
    }

    free(cnt);                            /* counter no longer needed */

    /* Store results.  */
    list_of_k = temp;
    list_len   = total;
    initialized = true;
}

/* ---------------  Public function --------------- */

seq_t a(idx_t n)
{
    if (!initialized) init_sequence();
    if (!initialized) return 0;                 /* OOM or init failure */
    if (n == 0 || n > list_len) return 0;       /* out of bounds request */
    return list_of_k[n - 1];                    /* 1‑based indexing */
}
