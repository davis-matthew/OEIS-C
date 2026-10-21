/*
 * A098762  This sequence, the sequence of first differences and the sequence
 *          of partial sums, are disjoint; a(0) = 1.
 * https://oeis.org/A098762
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * This implementation builds A098762 using a greedy construction:
 * a(0) = 1.
 * For n > 0 let a(n) be the smallest integer > a(n‑1) such that
 *  • a(n) is not already used,
 *  • the difference d = a(n) – a(n‑1) is not already used,
 *  • the sum  s = a(n) + a(n‑1) is not already used.
 * The set “used” contains all terms, all previous differences and all
 * previous sums.  The algorithm guarantees the three sequences are
 * pairwise disjoint and yields the sequence listed in OEIS A098762.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* Forward declarations for internal helpers */
static void ensure_used(size_t val);
static void ensure_seq_size(idx_t n);

/* Static storage for generated terms and the used–number bitmap */
static seq_t *seq = NULL;
static size_t seq_cap = 0;           /* capacity of seq array            */
static size_t seq_len = 0;           /* number of filled entries in seq   */

static bool *used = NULL;
static size_t used_cap = 0;          /* bit‑array capacity (in elements) */

/* Ensure the used bitmap can hold the value val (0‑based index) */
static void ensure_used(size_t val)
{
    if (val >= used_cap) {
        size_t new_cap = used_cap ? used_cap << 1 : 16;
        while (new_cap <= val) new_cap <<= 1;
        bool *tmp = (bool *)realloc(used, new_cap * sizeof(bool));
        if (!tmp) abort();                      /* out of memory */
        /* initialise the new part to false */
        memset(tmp + used_cap, 0, (new_cap - used_cap) * sizeof(bool));
        used = tmp;
        used_cap = new_cap;
    }
}

/* Ensure the seq array can hold at least (n+1) elements */
static void ensure_seq_size(idx_t n)
{
    if ((size_t)n >= seq_cap) {
        size_t new_cap = seq_cap ? seq_cap << 1 : 32;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        if (!tmp) abort();                      /* out of memory */
        seq = tmp;
        seq_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* Initialise on the first call */
    if (seq_len == 0) {
        /* Allocate initial storage */
        seq_cap = 64;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        seq_len = 0;

        used_cap = 16;
        used = (bool *)calloc(used_cap, sizeof(bool));
        if (!seq || !used) abort();               /* out of memory */

        /* First term */
        seq[seq_len++] = (seq_t)1;
        /* Mark the initial number as used */
        ensure_used(1);
        used[1] = true;
    }

    /* Generate sequence up to index n if needed */
    while (seq_len <= (size_t)n) {
        seq_t prev = seq[seq_len - 1];
        seq_t cand = prev + 1;          /* candidate > prev */

        for (;;) {
            seq_t diff = cand - prev;
            seq_t sum  = cand + prev;

            /* Ensure bitmap can contain required indices */
            ensure_used((size_t)cand);
            ensure_used((size_t)diff);
            ensure_used((size_t)sum);

            /* Accept candidate if it is not already used */
            if (!used[(size_t)cand] &&
                !used[(size_t)diff] &&
                !used[(size_t)sum]) {
                break;
            }
            cand++;
        }

        /* Insert the new term and maintain storage */
        ensure_seq_size(seq_len);
        seq[seq_len++] = cand;

        /* Mark the new term, its difference and sum as used */
        used[(size_t)cand] = true;
        used[(size_t)(cand - prev)] = true;
        used[(size_t)(cand + prev)] = true;
    }

    return seq[n];
}
