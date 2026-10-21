/*
 * A086287  Greatest prime factor of 7-smooth numbers.
 * https://oeis.org/A086287
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = greatest prime factor of the n‑th 7‑smooth number
 *            (A002473(n))
 * Range:     u8 n <= 76, u16 n <= 1697, u32 n <= 6719, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static storage for the sequence of 7‑smooth numbers */
static seq_t *h = NULL;          /* array of Hamming numbers            */
static idx_t h_capacity = 0;     /* allocated size of h                */
static idx_t h_count = 0;        /* number of values stored            */

static idx_t p2 = 0, p3 = 0, p5 = 0, p7 = 0;   /* pointers for the four multipliers */

/* Ensure that the array h contains at least n values */
static void ensure_n(idx_t n)
{
    if (h_count >= n) return;

    /* Grow the array if necessary */
    if (h_capacity < n) {
        size_t newcap = h_capacity ? h_capacity << 1 : 1;
        while (newcap < (size_t)n) newcap <<= 1;
        h = (seq_t *)realloc(h, newcap * sizeof(seq_t));
        h_capacity = newcap;
    }

    /* First initialisation: the very first 7‑smooth number is 1 */
    if (h_count == 0) {
        h[0] = 1;
        h_count = 1;
        p2 = p3 = p5 = p7 = 0;
    }

    /* Generate numbers up to index n-1 (0‑based) */
    for (; h_count < n; ++h_count) {
        seq_t c2 = h[p2] * 2;
        seq_t c3 = h[p3] * 3;
        seq_t c5 = h[p5] * 5;
        seq_t c7 = h[p7] * 7;

        seq_t next = c2;
        if (c3 < next) next = c3;
        if (c5 < next) next = c5;
        if (c7 < next) next = c7;

        h[h_count] = next;

        if (next == c2) ++p2;
        if (next == c3) ++p3;
        if (next == c5) ++p5;
        if (next == c7) ++p7;
    }
}

/* Return the largest prime factor among {2,3,5,7} that divides x */
static seq_t largest_prime_factor(seq_t x)
{
    if (x == 1) return 1;
    if (x % 7 == 0) return 7;
    if (x % 5 == 0) return 5;
    if (x % 3 == 0) return 3;
    if (x % 2 == 0) return 2;
    return 1;          /* should never happen for 7‑smooth numbers */
}

/* Public sequence function */
seq_t a(idx_t n)
{
    /* OEIS offset 1: a(1) is the first term */
    if (n == 0) return 0;  /* undefined, but never called */
    ensure_n(n);
    seq_t val = h[n - 1];
    return largest_prime_factor(val);
}
