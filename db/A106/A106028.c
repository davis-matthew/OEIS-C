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
 * Helpers:   bitlen_seq, has_consecutive_ones, lev_dist
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the minimal number of edit (insert, delete, substitute) steps
   required to transform the binary representation of n into that of the
   n‑th fibbinary number. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/bitlen_seq.h"
#include "../utils/has_consecutive_ones.h"
#include "../utils/lev_dist.h"

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
        fibb_arr = (seq_t *)malloc(fibb_cap * sizeof(seq_t));
    } else if (fibb_size >= fibb_cap) {
        fibb_cap <<= 1;                     /* double the capacity */
        fibb_arr = (seq_t *)realloc(fibb_arr,
                                    fibb_cap * sizeof(seq_t));
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
