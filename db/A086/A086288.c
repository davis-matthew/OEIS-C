/*
 * A086288  Number of distinct prime factors of 7-smooth numbers.
 * https://oeis.org/A086288
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct prime factors of the n‑th 7‑smooth
 *            number (primes allowed: 2,3,5,7). The sequence is obtained by
 *            generating all 7‑smooth numbers in increasing order and, for
 *            each, counting how many of 2,3,5,7 divide it. Because only these
 *            primes can appear, the count is simply the numbe...
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t  *counts = NULL;   /* a(n) values */
static uint64_t *values = NULL; /* 7‑smooth numbers */
static idx_t   size = 0;        /* number of values stored (>=1) */
static idx_t   capacity = 0;    /* allocated array size */
static idx_t   i2 = 0, i3 = 0, i5 = 0, i7 = 0; /* tiny indices for candidates */

/* initialise arrays with first 7‑smooth number (1) */
static void init(void)
{
    if (!counts) {
        capacity = 1024;
        values  = (uint64_t *)malloc(sizeof(uint64_t) * capacity);
        counts  = (seq_t   *)malloc(sizeof(seq_t   ) * capacity);
        values[0] = 1;
        counts[0] = 0;   /* a(1) = 0 */
        size = 1;
    }
}

/* double the storage */
static void grow(void)
{
    capacity *= 2;
    values = (uint64_t *)realloc(values, sizeof(uint64_t) * capacity);
    counts = (seq_t   *)realloc(counts, sizeof(seq_t   ) * capacity);
}

/* generate values up to at least n */
static void ensure(idx_t n)
{
    init();
    while (size < n) {
        /* compute smallest next candidate */
        unsigned __int128 c2 = (unsigned __int128)values[i2] * 2;
        unsigned __int128 c3 = (unsigned __int128)values[i3] * 3;
        unsigned __int128 c5 = (unsigned __int128)values[i5] * 5;
        unsigned __int128 c7 = (unsigned __int128)values[i7] * 7;
        unsigned __int128 next = c2;
        if (c3 < next) next = c3;
        if (c5 < next) next = c5;
        if (c7 < next) next = c7;

        uint64_t val = (uint64_t)next;  /* fits in 64 bits for n up to ~10⁵ */

        if (size >= capacity) grow();

        values[size] = val;
        seq_t cnt = 0;
        if (val % 2 == 0) cnt++;
        if (val % 3 == 0) cnt++;
        if (val % 5 == 0) cnt++;
        if (val % 7 == 0) cnt++;
        counts[size] = cnt;
        size++;

        /* move indices whose candidate was chosen */
        if (next == c2) i2++;
        if (next == c3) i3++;
        if (next == c5) i5++;
        if (next == c7) i7++;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* out of bounds handled silently */
    ensure(n);
    return counts[n - 1];
}
