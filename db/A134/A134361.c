/*
 * A134361  a(n) = smallest integer >= n which has only prime factors 2 and 3.
 * https://oeis.org/A134361
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = smallest integer ≥ n that has only prime factors 2 and 3
 *            (3‑smooth)
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache of 3‑smooth numbers */
static seq_t *smooth = NULL;      /* array of computed 3‑smooth numbers */
static size_t smooth_len = 0;     /* number of elements stored */
static size_t smooth_cap = 0;     /* allocated capacity */
static size_t i2 = 0, i3 = 0;     /* indices for generating next multiples */

/* Ensure that the cache is initialized and has enough space. */
static void smooth_init(void)
{
    if (smooth == NULL) {
        smooth_cap = 16;
        smooth = (seq_t *)malloc(smooth_cap * sizeof(seq_t));
        if (smooth == NULL) { /* allocation failed – abort with zero */
            smooth_len = 0;
            return;
        }
        smooth[0] = 1;          /* 3‑smooth numbers start with 1 */
        smooth_len = 1;
        i2 = i3 = 0;
    }
}

static void smooth_grow(size_t min_cap)
{
    if (min_cap > smooth_cap) {
        size_t new_cap = smooth_cap ? smooth_cap * 2 : 1;
        while (new_cap < min_cap) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(smooth, new_cap * sizeof(seq_t));
        if (tmp == NULL) return;     /* keep old memory on failure */
        smooth = tmp;
        smooth_cap = new_cap;
    }
}

/* Generate the next 3‑smooth number(s) until we reach or exceed target. */
static void smooth_fill_until(seq_t target)
{
    smooth_init();
    while (smooth_len > 0 && smooth[smooth_len - 1] < target) {
        seq_t next2 = smooth[i2] * 2;
        seq_t next3 = smooth[i3] * 3;
        seq_t next = (next2 < next3) ? next2 : next3;
        if (next == next2) ++i2;
        if (next == next3) ++i3;

        /* Avoid duplicate insertion */
        if (smooth_len == 0 || smooth[smooth_len - 1] != next) {
            smooth_grow(smooth_len + 1);
            smooth[smooth_len++] = next;
        }
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* undefined domain, return zero for safety */
    seq_t target = (seq_t)n;
    smooth_fill_until(target);
    return smooth_len > 0 ? smooth[smooth_len - 1] : 0;
}
