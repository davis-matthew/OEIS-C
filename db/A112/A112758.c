/*
 * A112758  Number of distinct prime factors of n-th 5-smooth number.
 * https://oeis.org/A112758
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct prime factors of the nth 5‑smooth
 *            number
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static state holding the generated 5‑smooth numbers */
    static uint64_t *smooth = NULL;   /* array of generated numbers */
    static size_t smooth_cap = 0;     /* current allocated capacity */
    static idx_t smooth_len = 0;      /* number of numbers stored */
    static idx_t i2 = 0, i3 = 0, i5 = 0;   /* indices for multiples of 2, 3, 5 */
    static int initialized = 0;

    /* One‑time initialization */
    if (!initialized) {
        smooth_cap = 128;
        smooth = (uint64_t*)calloc(smooth_cap, sizeof(uint64_t));
        if (!smooth) return (seq_t)0;   /* graceful failure if OOM */
        smooth[0] = 1;                 /* first 5‑smooth number */
        smooth_len = 1;
        initialized = 1;
    }

    /* Grow the array until we have at least n entries */
    while (smooth_len < n) {
        /* Resize if needed */
        if ((size_t)smooth_len >= smooth_cap) {
            size_t new_cap = smooth_cap * 2;
            uint64_t *tmp = (uint64_t*)realloc(smooth, new_cap * sizeof(uint64_t));
            if (!tmp) return (seq_t)0;  /* graceful failure if OOM */
            smooth = tmp;
            smooth_cap = new_cap;
        }

        /* Compute next 5‑smooth number */
        uint64_t c2 = smooth[i2] * 2ULL;
        uint64_t c3 = smooth[i3] * 3ULL;
        uint64_t c5 = smooth[i5] * 5ULL;

        uint64_t next = c2;
        if (c3 < next) next = c3;
        if (c5 < next) next = c5;

        smooth[smooth_len++] = next;

        /* Increment indices that produced the chosen value (handle duplicates) */
        if (next == c2) ++i2;
        if (next == c3) ++i3;
        if (next == c5) ++i5;
    }

    /* Count distinct prime factors (only 2, 3, 5 may divide a 5‑smooth number) */
    uint64_t val   = smooth[n - 1];
    seq_t count = 0;
    if ((val & 1ULL) == 0ULL) ++count;          /* divisible by 2 */
    if (val % 3ULL == 0ULL) ++count;            /* divisible by 3 */
    if (val % 5ULL == 0ULL) ++count;            /* divisible by 5 */

    return count;
}
