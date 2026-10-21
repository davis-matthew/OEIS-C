/*
 * A073662  Rearrangement of even numbers such that a(k)*a(k+1) + 1 is a prime
 *          for all k.
 * https://oeis.org/A073662
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 115, u32 n <= 116, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   alloc_seq_arr, alloc_used_arr, is_prime_big, realloc_seq_arr,
 *            realloc_used_arr
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sequence A073662.
   a(1)=2; for n>1, a(n) is the smallest even integer not yet used
   such that a(n‑1)*a(n)+1 is prime.  The function works for any
   integer type seq_t (unsigned 8–128 bits or an arbitrary‑precision
   integer class) and any idx_t type (uint64_t).  It is safe for all
   order of calls. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/is_prime_big.h"
#include "../utils/alloc_seq_arr.h"
#include "../utils/alloc_used_arr.h"
#include "../utils/realloc_seq_arr.h"
#include "../utils/realloc_used_arr.h"

/* static state ------------------------------------------------------------- */

static seq_t *seq      = NULL;   /* array of terms a(1)..a(len)      */
static size_t  seq_len = 0;     /* number of terms stored           */
static size_t  seq_cap = 0;     /* capacity of the seq array        */

static uint8_t *used     = NULL;   /* used[even/2] = 1 if that even
                                      number has already appeared      */
static size_t   used_cap = 0;       /* capacity of the used array       */

/* Ensure the used array can hold the index idx (i.e. even number 2*idx). */
static void ensure_used_capacity(size_t idx)
{
    if (idx < used_cap) return;
    size_t new_cap = used_cap == 0 ? 4 : used_cap << 1;
    while (new_cap <= idx) new_cap <<= 1;
    uint8_t *new_used = (uint8_t*)realloc_used_arr(used, used_cap, new_cap);
    if (!new_used) abort();
    /* zero the freshly allocated tail */
    memset(new_used + used_cap, 0, (new_cap - used_cap) * sizeof(uint8_t));
    used = new_used;
    used_cap = new_cap;
}

/* Ensure the sequence is computed up to (and including) position target. */
static void extend_to(idx_t target)
{
    /* initialise on first call */
    if (seq == NULL) {
        seq_cap = 8;
        seq = (seq_t*)alloc_seq_arr(seq_cap);
        if (!seq) abort();
        seq[0] = (seq_t)2;          /* a(1) = 2   */
        seq_len = 1;

        used_cap = 4;
        used = (uint8_t*)alloc_used_arr(used_cap);
        if (!used) abort();
        memset(used, 0, used_cap * sizeof(uint8_t));
        used[1] = 1;                /* even number 2 is used   */
    }

    while ((idx_t)seq_len < target) {
        seq_t prev = seq[seq_len - 1];
        seq_t chosen = 0;
        uint64_t num = 2; /* start with the smallest even number */

        /* search for the smallest unused even number such that
           prev * candidate + 1 is prime */
        while (true) {
            size_t idx = num >> 1;            /* map 2*idx -> even num */
            ensure_used_capacity(idx);
            if (used[idx]) { num += 2; continue; }

            seq_t candidate = (seq_t)num;
            seq_t prod = prev * candidate + (seq_t)1;
            if (is_prime_big(prod)) {
                chosen = candidate;
                used[idx] = 1;
                break;
            }
            num += 2;
        }

        /* grow the seq array if necessary */
        if (seq_len >= seq_cap) {
            size_t new_cap = seq_cap == 0 ? 8 : seq_cap << 1;
            seq_t *new_seq = (seq_t*)realloc_seq_arr(seq, seq_cap, new_cap);
            if (!new_seq) abort();
            seq = new_seq;
            seq_cap = new_cap;
        }

        seq[seq_len++] = chosen;
    }
}

/* ------------------------------------------------------------------------- */
/* public function required by the specification                          */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset 1: a(1) is the first term   */
    extend_to(n);
    return seq[n - 1];
}
