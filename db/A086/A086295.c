/*
 * A086295  Sum of all prime factors of 7-smooth numbers.
 * https://oeis.org/A086295
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 389, u16 n <= 701, u32 n <= 8057, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = sum of all prime factors (with multiplicity) of the nth 7‑smooth number
 *  (numbers whose prime factors are only 2,3,5 and 7).  The nth 7‑smooth
 *  numbers are generated in increasing order using the standard Hamming‑number
 *  algorithm; for each new number we keep the sum of its prime factors.
 */

#include <stdlib.h>

static seq_t *h   = NULL;      /* 7‑smooth numbers, 1‑indexed */
static seq_t *spf = NULL;      /* sum of prime factors of h[i] */
static idx_t cap   = 0;        /* allocated capacity (>= size of arrays) */
static idx_t last  = 0;        /* largest index computed so far */

static idx_t i2 = 1, i3 = 1, i5 = 1, i7 = 1;  /* pointers for 2,3,5,7 */

static void ensure_capacity(idx_t need)
{
    if (need <= cap) return;
    idx_t newcap = cap ? cap : 16;
    while (newcap < need) newcap <<= 1;
    h   = (seq_t *)realloc(h,   newcap * sizeof(seq_t));
    spf = (seq_t *)realloc(spf, newcap * sizeof(seq_t));
    cap = newcap;
}

static void init_sequence(void)
{
    if (cap == 0) {
        ensure_capacity(16);
        h[1]   = (seq_t)1;
        spf[1] = (seq_t)0;   /* 1 has no prime factors */
        last   = 1;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* not used by OEIS offset */

    init_sequence();

    while (last < n) {
        /* Generate next 7‑smooth number */
        seq_t v2 = h[i2] * ((seq_t)2);
        seq_t v3 = h[i3] * ((seq_t)3);
        seq_t v5 = h[i5] * ((seq_t)5);
        seq_t v7 = h[i7] * ((seq_t)7);

        seq_t next = v2;
        if (v3 < next) next = v3;
        if (v5 < next) next = v5;
        if (v7 < next) next = v7;

        ++last;
        ensure_capacity(last + 1);
        h[last] = next;

        /* Sum of prime factors */
        if (next == v2) {
            spf[last] = spf[i2] + ((seq_t)2);
        } else if (next == v3) {
            spf[last] = spf[i3] + ((seq_t)3);
        } else if (next == v5) {
            spf[last] = spf[i5] + ((seq_t)5);
        } else { /* next == v7 */
            spf[last] = spf[i7] + ((seq_t)7);
        }

        /* Advance pointers for all duplicates */
        if (next == v2) i2++;
        if (next == v3) i3++;
        if (next == v5) i5++;
        if (next == v7) i7++;
    }

    return spf[n];
}
