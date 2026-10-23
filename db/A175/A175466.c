/*
 * A175466  Table read by antidiagonals: a(m,n) = the largest positive integer
 *          occurring, when written in binary, as a substring in both binary m
 *          and binary n.
 * https://oeis.org/A175466
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, tabl
 * Formula:   a(n) = value read by antidiagonals of table a(m,n), where *
 *            a(m,n) is the largest integer whose binary representation *
 *            occurs as a contiguous substring in both binary m and binary n
 * Range:     u8 n <= 9943, u16 n <= 9943, u32 n <= 9943, u64 n <= 9943,
 *            u128 n <= 9943, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>      /* for size_t */
#include <stdbool.h>     /* for bool */

/* Return the number of bits needed to represent x (0 -> 0). */
static unsigned bitlen_seq_t(seq_t x)
{
    unsigned len = 0;
    while (x != (seq_t)0) {
        ++len;
        x >>= 1;
    }
    return len;
}

/* Return a mask with the lowest 'len' bits set to 1.
 * Handles the case len == width of seq_t. */
static seq_t mask_for_len(unsigned len)
{
    unsigned width = sizeof(seq_t) * 8;
    if (len >= width) {
        return ~(seq_t)0;
    }
    return ((seq_t)1 << len) - (seq_t)1;
}

/* Largest integer whose binary string occurs as a substring in both
 * binary representations of m and n.  Assumes m,n >= 1. */
static seq_t common_max(seq_t m, seq_t n)
{
    if (m == (seq_t)0 || n == (seq_t)0)
        return (seq_t)0;

    seq_t minmn = m < n ? m : n;

    /* Search descending from minmn to 1. */
    for (seq_t x = minmn; x >= (seq_t)1; --x) {
        unsigned lenx = bitlen_seq_t(x);
        unsigned lenm = bitlen_seq_t(m);
        unsigned lenn = bitlen_seq_t(n);

        /* Check if x appears in m. */
        bool foundm = false;
        if (lenx <= lenm) {
            unsigned maxpos = lenm - lenx;
            for (unsigned pos = 0; pos <= maxpos; ++pos) {
                seq_t sub = (m >> (lenm - pos - lenx)) & mask_for_len(lenx);
                if (sub == x) {
                    foundm = true;
                    break;
                }
            }
        }
        if (!foundm) continue;

        /* Check if x appears in n. */
        bool foundn = false;
        if (lenx <= lenn) {
            unsigned maxpos = lenn - lenx;
            for (unsigned pos = 0; pos <= maxpos; ++pos) {
                seq_t sub = (n >> (lenn - pos - lenx)) & mask_for_len(lenx);
                if (sub == x) {
                    foundn = true;
                    break;
                }
            }
        }

        if (foundn) return x;   /* x is the largest common substring value. */
    }

    return (seq_t)1; /* Fallback – at least 1 appears in every binary string. */
}

/* Antidiagonal mapping: from a linear index n (offset 1) to (m,n) pair. */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;      /* undefined, but harmless */

    /* Find the smallest d such that d*(d+1)/2 >= n. */
    idx_t d = 1;
    idx_t t = 1;                 /* triangular number T(d) = d*(d+1)/2 */
    while (t < n) {
        ++d;
        t = d * (d + 1) / 2;
    }

    /* Position along the diagonal (1‑based). */
    idx_t start_prev = (d - 1) * d / 2;   /* T(d-1) */
    idx_t i = n - start_prev;             /* m */
    idx_t j = d - i + 1;                  /* n */

    return common_max((seq_t)i, (seq_t)j);
}
