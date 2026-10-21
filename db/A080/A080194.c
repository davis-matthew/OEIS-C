/*
 * A080194  7-smooth numbers which are not 5-smooth.
 * https://oeis.org/A080194
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the n-th smallest integer whose prime factors are ≤7 and
 *            which is divisible by 7 (i.e. a 4‑smooth number with a 7 factor)
 * Range:     u8 n <= 24, u16 n <= 329, u32 n <= 4775, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Static state for on‑demand generation of the sequence. */
static seq_t *h      = NULL; /* all 4‑smooth numbers (2,3,5,7) */
static size_t h_len   = 0, h_cap   = 0;
static seq_t *h7     = NULL; /* 4‑smooth numbers that are divisible by 7 */
static size_t h7_len = 0, h7_cap   = 0;
static size_t i2 = 0, i3 = 0, i5 = 0, i7 = 0;

/* Ensure the array h has room for at least one more element. */
static void ensure_h_capacity(void)
{
    if (h_cap <= h_len) {
        size_t new_cap = h_cap ? h_cap << 1 : 256;
        seq_t *nv = (seq_t *)realloc(h, new_cap * sizeof(seq_t));
        if (!nv) abort();
        h = nv;
        h_cap = new_cap;
    }
}

/* Ensure the array h7 has room for at least one more element. */
static void ensure_h7_capacity(void)
{
    if (h7_cap <= h7_len) {
        size_t new_cap = h7_cap ? h7_cap << 1 : 256;
        seq_t *nv = (seq_t *)realloc(h7, new_cap * sizeof(seq_t));
        if (!nv) abort();
        h7 = nv;
        h7_cap = new_cap;
    }
}

/* Generate 4‑smooth numbers until at least target_h7 numbers that are
   divisible by 7 have been accumulated. */
static void generate_until_h7(size_t target_h7)
{
    if (h_len == 0) {                     /* initialise */
        ensure_h_capacity();
        h[0] = (seq_t)1;
        h_len = 1;
    }

    while (h7_len < target_h7) {
        seq_t c2 = h[i2] * ((seq_t)2);
        seq_t c3 = h[i3] * ((seq_t)3);
        seq_t c5 = h[i5] * ((seq_t)5);
        seq_t c7 = h[i7] * ((seq_t)7);
        seq_t next = c2;
        if (c3 < next) next = c3;
        if (c5 < next) next = c5;
        if (c7 < next) next = c7;

        ensure_h_capacity();
        h[h_len++] = next;

        if (c2 == next) ++i2;
        if (c3 == next) ++i3;
        if (c5 == next) ++i5;
        if (c7 == next) ++i7;

        if ((next % ((seq_t)7)) == 0) {
            ensure_h7_capacity();
            h7[h7_len++] = next;
        }
    }
}

/* Return the n-th term (1‑based) of the A080194 sequence. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    size_t target = (size_t)n;
    generate_until_h7(target);
    return h7[target - 1];
}
