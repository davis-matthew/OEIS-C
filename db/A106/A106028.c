/*
 * A106028  Minimal number of editing steps (delete, insert or substitute) to
 *          transform the binary representation of n into that of A003714(n),
 *          the n-th fibbinary number.
 * https://oeis.org/A106028
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = LevenshteinDistance(A007088(n), A007088(A003714(n)))
 * Range:     u8 n <= 54, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Validated: partial, pass, 105 terms through n = 105 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the minimal number of edit (insert, delete, substitute) steps
   required to transform the binary representation of n into that of the
   n‑th fibbinary number. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/*--- Compute the number of bits in a seq_t value ---------------------*/
static idx_t bitlen_seq(seq_t x)
{
    idx_t len = 0;
    while (x != (seq_t)0) {
        ++len;
        x >>= 1;
    }
    return len ? len : 1;            /* never zero for n ≥ 1 */
}

/*--- Test whether an integer has consecutive 1‑bits ------------------*/
static bool has_consecutive_ones(idx_t x)
{
    return (x & (x << 1)) != 0;
}

/*--- Levenshtein distance between two bit strings -------------------*/
static int lev_dist(seq_t x, seq_t y)
{
    /* Maximal bit length considered (sufficient for all test ranges) */
    const int MAXL = 1024;

    int prev[MAXL + 1];
    int curr[MAXL + 1];

    idx_t lenx = bitlen_seq(x);
    idx_t leny = bitlen_seq(y);

    /* Safety check – should never hit for the problem domain */
    if (lenx > (idx_t)MAXL || leny > (idx_t)MAXL)
        return -1;

    for (int j = 0; j <= (int)leny; ++j)
        prev[j] = j;

    for (idx_t i = 1; i <= lenx; ++i) {
        curr[0] = (int)i;
        /* Extract i-th bit from the most significant side */
        seq_t bitx = (x >> (lenx - i)) & (seq_t)1u;
        int bx = (int)bitx;

        for (idx_t j = 1; j <= leny; ++j) {
            seq_t bity = (y >> (leny - j)) & (seq_t)1u;
            int by = (int)bity;
            int cost = (bx == by) ? 0 : 1;

            int del = prev[j] + 1;
            int ins = curr[j - 1] + 1;
            int sub = prev[j - 1] + cost;

            int v = del;
            if (ins < v) v = ins;
            if (sub < v) v = sub;
            curr[j] = v;
        }
        for (int j = 0; j <= (int)leny; ++j)
            prev[j] = curr[j];
    }
    return prev[(int)leny];
}

/*--- Global storage for fibbinary numbers (1‑based) ------------------*/
static seq_t *fibb_arr      = NULL;
static idx_t  fibb_cap      = 0;
static idx_t  fibb_size     = 0;
static idx_t  next_candidate = 1;          /* next integer to test */

/*--- Increase the capacity of the fibbinary array -------------------*/
static void ensure_fibb(void)
{
    if (fibb_cap == 0) {
        fibb_cap = 16;
        fibb_arr = mem_new(seq_t, fibb_cap);
    } else if (fibb_size >= fibb_cap) {
        fibb_cap <<= 1;                     /* double the capacity */
        fibb_arr = mem_resize(seq_t, fibb_arr, fibb_cap);
    }
}

/*--- Return the n‑th fibbinary number (1‑based) ---------------------*/
static seq_t fibbinary_at(idx_t n)
{
    while (fibb_size < n) {
        /* find next integer without consecutive 1's */
        while (has_consecutive_ones(next_candidate))
            ++next_candidate;

        ensure_fibb();
        fibb_arr[fibb_size++] = (seq_t)next_candidate;
        ++next_candidate;            /* advance for future calls */
    }
    return fibb_arr[n - 1];
}

/*--- OEIS sequence function ------------------------------------------*/
seq_t a(idx_t n)
{
    seq_t fibb = fibbinary_at(n);      /* n‑th fibbinary number   */
    int  dist = lev_dist((seq_t)n, fibb); /* Levenshtein distance   */
    return (seq_t)dist;
}
