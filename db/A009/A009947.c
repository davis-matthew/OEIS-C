/*
 * A009947  Sequence of nonnegative integers, but insert n/2 after every even
 *          number n.
 * https://oeis.org/A009947
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 383, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A009947: Sequence of nonnegative integers, but insert n/2 after every even number n.
   The sequence is built iteratively and cached, so calls can be in any order.
   The algorithm keeps track of the next natural number and whether a half
   of the last even number is pending. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static seq_t *seq = NULL;          /* cache of computed terms   */
static size_t seq_cap = 0;         /* allocated capacity        */
static idx_t  seq_len = 0;         /* number of terms stored    */
static idx_t  nat = 0;             /* next natural number to emit */
static bool   need_half = false;   /* whether the next term is nat/2 */

/* Ensure the cache has space for at least one more element. */
static void seq_ensure_cap(void)
{
    if (seq_len >= seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 256;
        seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);   /* out of memory */
        seq = tmp;
        seq_cap = new_cap;
    }
}

/* Append a value to the cache. */
static void seq_push(seq_t val)
{
    seq_ensure_cap();
    seq[seq_len++] = val;
}

/* Return the nth term of the sequence. */
seq_t a(idx_t n)
{
    /* First call: allocate the cache. */
    if (!seq) {
        seq_cap = 256;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        if (!seq) exit(EXIT_FAILURE);
    }

    while (seq_len <= n) {
        if (need_half) {
            /* Emit the half of the previous even number. */
            seq_push((seq_t)(nat >> 1));
            need_half = false;
            nat++;            /* advance to the next natural number */
        } else {
            /* Emit the current natural number. */
            seq_push((seq_t)nat);
            if ((nat & 1) == 0) {
                /* Even number: schedule its half for the next term. */
                need_half = true;
            } else {
                /* Odd number: advance immediately. */
                nat++;
            }
        }
    }

    return seq[n];
}
