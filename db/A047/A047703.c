/*
 * A047703  Numbers that are the sum of 4 but no fewer positive cubes.
 * https://oeis.org/A047703
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 1000, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   int_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers that are the sum of 4 but no fewer positive cubes.
   The algorithm enumerates all sums of 4 cubes up to an upper bound,
   then keeps only those that can't be expressed with 1, 2 or 3 cubes. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/int_cube_root.h"

static seq_t *seq = NULL;          /* sequence values */
static size_t seq_len = 0;         /* number of terms stored */
static size_t seq_cap = 0;         /* allocated capacity */
static bool computed = false;     /* initialization flag */

/* Ensure the internal sequence array has at least `min_cap` capacity */
static void ensure_capacity(size_t min_cap)
{
    if (seq_cap < min_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 64;
        if (new_cap < min_cap) new_cap = min_cap;
        seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        seq_cap = new_cap;
    }
}

/* Pre‑compute the sequence up to a fixed upper bound */
static void compute_sequence(void)
{
    /* Upper bound chosen to comfortably cover the first 10 000 terms. */
    const idx_t UPPER = 2000000ULL;           /* 2 000 000  */
    const idx_t maxCube = int_cube_root(UPPER);

    /* Allocate cubes (1‑based). */
    seq_t *cube = (seq_t *)malloc((maxCube + 1) * sizeof(seq_t));
    for (idx_t i = 1; i <= maxCube; ++i)
        cube[i] = (seq_t)i * (seq_t)i * (seq_t)i;

    /* Bitsets for sums with 1, 2, 3 and 4 cubes.  Use 0/1 bytes. */
    unsigned char *seen1 = (unsigned char *)calloc((size_t)UPPER + 1, 1);
    unsigned char *seen2 = (unsigned char *)calloc((size_t)UPPER + 1, 1);
    unsigned char *seen3 = (unsigned char *)calloc((size_t)UPPER + 1, 1);
    unsigned char *seen4 = (unsigned char *)calloc((size_t)UPPER + 1, 1);

    /* Mark one‑cube sums. */
    for (idx_t i = 1; i <= maxCube; ++i) {
        seq_t v = cube[i];
        if (v <= UPPER) seen1[(size_t)v] = 1;
    }

    /* Mark two‑cube sums (unordered). */
    for (idx_t i = 1; i <= maxCube; ++i) {
        for (idx_t j = i; j <= maxCube; ++j) {
            seq_t v = cube[i] + cube[j];
            if (v > UPPER) break;
            seen2[(size_t)v] = 1;
        }
    }

    /* Mark three‑cube sums (unordered). */
    for (idx_t i = 1; i <= maxCube; ++i) {
        for (idx_t j = i; j <= maxCube; ++j) {
            seq_t sum12 = cube[i] + cube[j];
            if (sum12 > UPPER) break;
            for (idx_t k = j; k <= maxCube; ++k) {
                seq_t v = sum12 + cube[k];
                if (v > UPPER) break;
                seen3[(size_t)v] = 1;
            }
        }
    }

    /* Mark four‑cube sums (unordered). */
    for (idx_t i = 1; i <= maxCube; ++i) {
        for (idx_t j = i; j <= maxCube; ++j) {
            seq_t sum12 = cube[i] + cube[j];
            if (sum12 > UPPER) break;
            for (idx_t k = j; k <= maxCube; ++k) {
                seq_t sum123 = sum12 + cube[k];
                if (sum123 > UPPER) break;
                for (idx_t l = k; l <= maxCube; ++l) {
                    seq_t v = sum123 + cube[l];
                    if (v > UPPER) break;
                    seen4[(size_t)v] = 1;
                }
            }
        }
    }

    /* Extract the required terms into the dynamic array `seq`. */
    seq_len = 0;
    for (seq_t m = 1; m <= UPPER; ++m) {
        if (seen4[(size_t)m] &&
            !seen1[(size_t)m] &&
            !seen2[(size_t)m] &&
            !seen3[(size_t)m]) {
            ensure_capacity(seq_len + 1);
            seq[seq_len++] = m;
        }
    }

    /* Clean up temporary allocations. */
    free(cube);
    free(seen1);
    free(seen2);
    free(seen3);
    free(seen4);

    computed = true;
}

seq_t a(idx_t n)
{
    if (!computed) compute_sequence();
    /* n is 1‑based; our array is 0‑based.  */
    return seq[n - 1];
}
