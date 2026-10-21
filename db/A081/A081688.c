/*
 * A081688  0 followed by A030124 - 1.
 * https://oeis.org/A081688
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A081688
   a(0) = 0
   a(n) = mex{ a(i), b(i) : 0 <= i < n }
   b(0) = 0
   b(n) = b(n‑1) + a(n) + 1
*/

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static uint8_t *bitset = NULL;   /* bitset marks used integers */
static idx_t   bitset_cap = 0;   /* capacity in bytes */

/* Ensure the bitset can store the bit for integer idx. */
static void ensure_bits(idx_t idx)
{
    size_t need   = (size_t)(idx >> 3);       /* required byte index */
    if (need >= bitset_cap) {
        size_t new_cap = bitset_cap ? bitset_cap : 8;
        while (need >= new_cap)
            new_cap <<= 1;
        uint8_t *tmp = (uint8_t *)realloc(bitset, new_cap);
        if (!tmp) abort();
        /* zero new region */
        memset(tmp + bitset_cap, 0, new_cap - bitset_cap);
        bitset   = tmp;
        bitset_cap = new_cap;
    }
}

/* Set bit for integer idx. */
static inline void set_bit(idx_t idx)
{
    ensure_bits(idx);
    bitset[(size_t)(idx >> 3)] |= 1u << (idx & 7);
}

/* Test whether integer idx is marked. */
static inline int is_set(idx_t idx)
{
    size_t word = (size_t)(idx >> 3);
    return (word < bitset_cap) ?
           ((bitset[word] >> (idx & 7)) & 1u) : 0;
}

/* Dynamic arrays for a(n) and b(n). */
static seq_t *a_arr = NULL;
static seq_t *b_arr = NULL;
static idx_t  a_cap   = 0;   /* capacity in elements */
static idx_t  n_used  = 0;   /* how many terms are ready */

/* Grow the a/b arrays to at least `needed` elements. */
static void ensure_capacity(idx_t needed)
{
    if (needed < a_cap) return;
    idx_t new_cap = a_cap ? a_cap : 4;
    while (needed >= new_cap) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(a_arr, new_cap * sizeof(*a_arr));
    if (!tmp) abort();
    a_arr = tmp;
    tmp = (seq_t *)realloc(b_arr, new_cap * sizeof(*b_arr));
    if (!tmp) abort();
    b_arr = tmp;
    a_cap = new_cap;
}

/* The next mex value that has not yet been used. */
static idx_t current_mex = 1;  /* starts after a[0]=b[0]=0 */

/* ------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Initialise on first call. */
    if (n_used == 0) {
        ensure_capacity(1);
        a_arr[0] = (seq_t)0;
        b_arr[0] = (seq_t)0;
        set_bit((idx_t)0);   /* mark 0 as used (a[0] and b[0]) */
        n_used = 1;
    }

    /* Compute terms up to n. */
    while (n_used <= n) {
        /* Find the next unused integer: current_mex is already the
           smallest candidate that hasn't been tested yet.  It may
           need adjustment if it is already used. */
        while (is_set(current_mex)) ++current_mex;

        seq_t aval = (seq_t)current_mex;   /* a[n_used] = mex */

        ensure_capacity(n_used + 1);
        a_arr[n_used] = aval;
        set_bit(current_mex);

        /* Compute b value. */
        seq_t bval = b_arr[n_used - 1] + aval + (seq_t)1;
        b_arr[n_used] = bval;
        set_bit((idx_t)bval);

        /* Update current_mex for next iteration. */
        ++current_mex;

        ++n_used;
    }

    return a_arr[n];
}
