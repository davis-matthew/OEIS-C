/*
 * A240002  Number of 3 X n 0..3 arrays with no element equal to one plus the
 *          sum of elements to its left or two plus the sum of the elements
 *          above it or one plus the sum of the elements diagonally to its
 *          northwest, modulo 4.
 * https://oeis.org/A240002
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 57, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A240002: 9*a(n-1) - 36*a(n-2) + 84*a(n-3) - 126*a(n-4)
   + 126*a(n-5) - 84*a(n-6) + 36*a(n-7) - 9*a(n-8) + a(n-9)   (n>15)
   Initial values: a(1)=12, a(2)=61, a(3)=190, a(4)=526, a(5)=1262,
   a(6)=2766, a(7)=5647, a(8)=10878, a(9)=19971, a(10)=35180,
   a(11)=59780, a(12)=98414, a(13)=157524, a(14)=245879,
   a(15)=375214. */

#include <stdlib.h>
#include <stdint.h>

static const seq_t init[15] = {
    12, 61, 190, 526, 1262,
    2766, 5647, 10878, 19971, 35180,
    59780, 98414, 157524, 245879, 375214
};

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* 1‑indexed cache; cache[0] unused */
    static idx_t max_n  = 0;      /* highest computed index */

    if (n < 1)                        /* safety for out‑of‑range call */
        return 0;

    /* Allocate or enlarge cache if necessary */
    if (n > max_n) {
        idx_t new_size = n + 1;                /* indices 0 .. n inclusive */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();                     /* memory allocation failed */
        cache = tmp;
    }

    /* Compute any terms that are not yet cached */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        seq_t val;
        if (i <= 15) {                        /* use the known initial terms */
            val = init[i - 1];
        } else {
            /* linear recurrence using unsigned arithmetic;
               intermediate sums are guaranteed to be non‑negative. */
            seq_t pos = 0, neg = 0;
            pos += cache[i - 1] * 9;
            pos += cache[i - 3] * 84;
            pos += cache[i - 5] * 126;
            pos += cache[i - 7] * 36;
            pos += cache[i - 9];
            neg += cache[i - 2] * 36;
            neg += cache[i - 4] * 126;
            neg += cache[i - 6] * 84;
            neg += cache[i - 8] * 9;
            val = pos - neg;       /* pos >= neg guarantees no underflow */
        }
        cache[i] = val;
    }
    max_n = n;
    return cache[n];
}
