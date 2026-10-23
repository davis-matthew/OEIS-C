/*
 * A108651  Multiples of 8 that are divisible by no prime > 5.
 * https://oeis.org/A108651
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0; for n≥1, a(n)=8*hamming[n-1] where hamming is the
 *            sequence of 5‑smooth numbers (1,2,3,4,5,6,8,9,10,12,…)
 * Range:     u8 n <= 18, u16 n <= 166, u32 n <= 700, u64 n <= 700,
 *            u128 n <= 700, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *h = NULL;          /* array of 5‑smooth numbers, 0‑based */
static idx_t h_count = 0;        /* number of elements stored in h */
static idx_t idx2 = 0, idx3 = 0, idx5 = 0;  /* indices for multiples by 2,3,5 */
static size_t h_cap = 0;         /* current capacity of h */

static void ensure_capacity(size_t needed)
{
    if (needed <= h_cap)
        return;
    size_t new_cap = (h_cap == 0) ? 8u : (h_cap << 1);
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(h, new_cap * sizeof(seq_t));
    if (!tmp && new_cap > 0)
        abort();
    h = tmp;
    h_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* offset 0: the first term is 0 */
    if (n == 0)
        return (seq_t)0;

    /* initialise on first invocation */
    if (!h) {
        ensure_capacity(8);
        h[0] = (seq_t)1;          /* first 5‑smooth number */
        h_count = 1;
        idx2 = idx3 = idx5 = 0;
    }

    /* index of the required 5‑smooth number (0‑based) */
    idx_t target = n - 1;

    /* generate until we have target+1 numbers */
    if (h_count <= target) {
        ensure_capacity(target + 1);
        while (h_count <= target) {
            seq_t next2 = h[idx2] * (seq_t)2;
            seq_t next3 = h[idx3] * (seq_t)3;
            seq_t next5 = h[idx5] * (seq_t)5;
            seq_t next = next2;
            if (next3 < next) next = next3;
            if (next5 < next) next = next5;
            h[h_count] = next;
            ++h_count;
            if (next == next2) ++idx2;
            if (next == next3) ++idx3;
            if (next == next5) ++idx5;
        }
    }

    return (seq_t)8 * h[target];
}
