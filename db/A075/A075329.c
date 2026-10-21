/*
 * A075329  Pair up the natural numbers as (r, s) (say) so that all natural
 *          numbers are obtained only once as r, s, s+r or s-r: (1, 3), (5,
 *          11), (7, 15), (9, 19), (12, 25), (14, 31), (18, 38), ... Sequence
 *          gives first member of each pair.
 * https://oeis.org/A075329
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm:  Greedy construction of the pairs (r,s).  For each new r
   choose the smallest unused s > r such that s, s+r, and s-r are all
   unused, then mark these four numbers as used.  The sequence a(n) is
   the first element r of the n‑th pair. */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

/* Persistent state ----------------------------------------------------- */
static uint8_t *used = NULL;      /* bitset of used natural numbers */
static size_t used_cap = 0;        /* allocated capacity for 'used' */
static idx_t next_r = 1;           /* next candidate r */

static seq_t *seq = NULL;          /* array of r values */
static size_t seq_cap = 0;         /* capacity of seq */
static size_t seq_len = 0;         /* number of r values stored */

/* Ensure that 'used' can address index i (inclusive).  Allocate in
   blocks of power of two. */
static void ensure_used_size(idx_t i)
{
    if ((size_t)i >= used_cap) {
        size_t new_cap = used_cap ? used_cap * 2 : 16;
        while ((size_t)i >= new_cap) new_cap *= 2;

        void *tmp = realloc(used, new_cap * sizeof(uint8_t));
        if (!tmp) exit(EXIT_FAILURE);
        /* Zero the newly allocated part. */
        if (new_cap > used_cap)
            memset((uint8_t*)tmp + used_cap, 0,
                   (new_cap - used_cap) * sizeof(uint8_t));
        used   = (uint8_t*)tmp;
        used_cap = new_cap;
    }
}

/* Ensure space for one more element in the result array. */
static void ensure_seq_cap(void)
{
    if (seq_len >= seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 16;
        void *tmp = realloc(seq, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        seq   = (seq_t*)tmp;
        seq_cap = new_cap;
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* undefined for n=0 – harmless */

    /* Generate terms until we have at least n of them. */
    while (seq_len < (size_t)n) {
        /* 1. Find smallest unused r. */
        idx_t r = next_r;
        ensure_used_size(r);
        while (used[(size_t)r]) {
            ++r;
            ensure_used_size(r);
        }
        next_r = r + 1;   /* next search starts after this r */

        /* 2. Find smallest s > r with s, s+r, s-r all unused. */
        seq_t s = (seq_t)2 * r + 1;   /* minimal possible s */
        for (;;) {
            idx_t s_idx  = (idx_t)s;
            idx_t sr_p   = (idx_t)(s + (seq_t)r);
            idx_t sr_m   = (idx_t)(s - (seq_t)r);

            ensure_used_size(s_idx);
            ensure_used_size(sr_p);
            ensure_used_size(sr_m);

            if (!used[(size_t)s_idx] &&
                !used[(size_t)sr_p] &&
                !used[(size_t)sr_m]) {
                break;   /* suitable s found */
            }
            ++s;
        }

        /* 3. Mark r, s, s+r, s-r as used. */
        used[(size_t)r] = 1;
        used[(size_t)(seq_t)s] = 1;
        used[(size_t)(seq_t)(s + (seq_t)r)] = 1;
        used[(size_t)(seq_t)(s - (seq_t)r)] = 1;

        /* 4. Store r in the result array. */
        ensure_seq_cap();
        seq[seq_len++] = r;
    }

    return seq[n - 1];
}
