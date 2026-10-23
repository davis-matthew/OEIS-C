/*
 * A169845  EKG sequence started at 7 instead of 2.
 * https://oeis.org/A169845
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 166, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements the EKG sequence started at 7.
   a(1) = 7, and for n > 1,
   a(n) = smallest unused positive integer such that
   gcd(a(n-1), a(n)) > 1. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/gcd.h"   /* provides seq_t gcd(seq_t a, seq_t b) */

/* ------------------------------------------------------------------ */
/* Static state for memoisation                                       */

static seq_t *seq = NULL;                /* array of generated terms      */
static size_t seq_cap = 0;               /* capacity of 'seq'             */
static size_t seq_len = 0;               /* number of terms stored (0‑based) */

/* Bitset recording numbers already used.
   Index i is set to 1 iff number i has appeared.      */
static uint8_t *seen = NULL;
static size_t seen_cap = 0;              /* maximum index handled        */

/* ------------------------------------------------------------------ */
/* Helper: ensure seq array has capacity 'need' (at least 'need' elements) */

static void ensure_seq_cap(size_t need)
{
    if (need <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 128;
    while (new_cap < need) new_cap *= 2;
    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/* Helper: ensure seen array can hold element 'idx' (set bit for idx)    */

static void ensure_seen_cap(size_t idx)
{
    if (idx < seen_cap) return;
    size_t new_cap = seen_cap ? seen_cap * 2 : 128;
    while (new_cap <= idx) new_cap *= 2;
    uint8_t *new_seen = (uint8_t *)realloc(seen, new_cap * sizeof(uint8_t));
    memset(new_seen + seen_cap, 0, new_cap - seen_cap); /* clear new part */
    seen = new_seen;
    seen_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/* Initialise the sequence the first time it is requested.               */

static void init_sequence(void)
{
    if (seq_len > 0) return;     /* already initialised */
    ensure_seq_cap(1);
    seq[0] = (seq_t)7;
    seq_len = 1;
    ensure_seen_cap(7);
    seen[7] = 1;
}

/* ------------------------------------------------------------------ */
/* Generate terms up to index 'n' (1‑based).                               */

static void generate_up_to(idx_t n)
{
    /* n is 1‑based; seq_len == current number of stored terms               */
    while ((idx_t)seq_len < n) {
        seq_t last = seq[seq_len - 1];
        idx_t candidate = 2;                      /* the smallest possible */
        for (;;) {
            if (candidate < 0) /* guard against overflow, though unlikely */ {
                /* If we run out of integers (theoretically impossible here),
                   return.                                                   */
                return;
            }
            ensure_seen_cap((size_t)candidate);
            if (!seen[(size_t)candidate]) {
                seq_t g = gcd(last, (seq_t)candidate);
                if (g > 1) break;                 /* valid candidate */
            }
            ++candidate;
        }
        /* Store the found term */
        ensure_seq_cap(seq_len + 1);
        seq[seq_len] = (seq_t)candidate;
        seen[(size_t)candidate] = 1;
        seq_len++;
    }
}

/* ------------------------------------------------------------------ */
/* Public function required by the build                                 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* behaviour for n < 1 is unspecified */
    init_sequence();
    generate_up_to(n);
    return seq[n - 1];
}
