/*
 * A133269  Fractal sequence consisting of quadruples, each of which contains
 *          the tones comprising a major 7th chord (i.e., root, major third,
 *          fifth, and major 7th), with the tones in an octave assigned to the
 *          numbers 1..12, and with the n-th quadruple using a(n) as its root.
 * https://oeis.org/A133269
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   not stated
 * Range:     u8 n <= 93, u16 n <= 93, u32 n <= 93, u64 n <= 93, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1,a(2)=5,a(3)=8,a(4)=12; for k>1
   a(4k-3)=a(k), a(4k-2)=(a(k)+3) mod 12 + 1,
   a(4k-1)=(a(k)+6) mod 12 + 1, a(4k)=(a(k)+10) mod 12 + 1. */
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cap = 0;

    /* Ensure cache is large enough for index n (1‑based). */
    if (cache == NULL) {
        cap = 16;
        cache = (seq_t *)calloc(cap, sizeof(seq_t));
    }
    if (n >= cap) {
        idx_t new_cap = cap;
        while (new_cap <= n)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;               /* allocation failure – unlikely */
        /* zero new elements (they are 0 => uncomputed) */
        for (idx_t i = cap; i < new_cap; ++i)
            tmp[i] = 0;
        cache = tmp;
        cap = new_cap;
    }

    /* Base values for n = 1..4. */
    if (n <= 4) {
        switch (n) {
            case 1: return 1;
            case 2: return 5;
            case 3: return 8;
            case 4: return 12;
        }
    }

    /* Return memoized result if present. */
    if (cache[n] != 0)
        return cache[n];

    /* Compute recursively using the recurrence. */
    idx_t k = (n + 3) / 4;               /* ceil(n/4) */
    seq_t ck = a(k);                     /* recursive call */
    idx_t r = n % 4;                     /* 1,2,3 or 0 */
    seq_t val;
    switch (r) {
        case 1: val = ck; break;
        case 2: val = (ck + 3) % 12 + 1; break;
        case 3: val = (ck + 6) % 12 + 1; break;
        default: /* r == 0 */ val = (ck + 10) % 12 + 1; break;
    }

    cache[n] = val;
    return val;
}
