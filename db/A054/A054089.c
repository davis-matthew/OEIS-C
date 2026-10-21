/*
 * A054089  For k >= 1, let p(k) = least h in N not already an a(i), q(k) =
 *          p(k)+k, a(2k) = q(k), a(2k+1) = p(k).
 * https://oeis.org/A054089
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* Sequence A054089: 
   p(k) = least unused positive integer, q(k) = p(k)+k, 
   a(2k) = q(k), a(2k+1) = p(k), with a(1)=1, offset 1. */

/* Memoisation state */
static seq_t *seq_cache = NULL;       /* 1‑based array of computed terms */
static size_t seq_len  = 0;           /* largest index computed (max n) */
static size_t seq_cap  = 0;           /* allocated capacity of seq_cache */

static unsigned char *used_flags = NULL; /* bit‑flags for used integers (index = integer) */
static size_t used_cap   = 0;               /* capacity of used_flags */
static idx_t next_unseen = 2;               /* next candidate integer */

static void ensure_cache_capacity(idx_t needed)
{
    if (needed > (idx_t)seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 16;
        while (new_cap < (size_t)needed) new_cap <<= 1;
        seq_t *new_cache = (seq_t *)realloc(seq_cache, (new_cap + 1) * sizeof(seq_t));
        seq_cache = new_cache;
        seq_cap   = new_cap;
    }
}

static void ensure_used_capacity(idx_t needed)
{
    if (needed > (idx_t)used_cap) {
        size_t new_cap = used_cap ? used_cap * 2 : 32;
        while (new_cap < (size_t)needed) new_cap <<= 1;
        unsigned char *new_used = (unsigned char *)realloc(used_flags, (new_cap + 1) * sizeof(unsigned char));
        /* Zero initialise newly allocated part */
        memset(new_used + used_cap + 1, 0, (new_cap - used_cap) * sizeof(unsigned char));
        used_flags = new_used;
        used_cap   = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* n must be >= 1 for meaningful input; return 0 for n==0 */
    if (n == 0) return 0;

    /* First‑time initialisation */
    if (seq_len == 0) {
        seq_cap  = 16;
        seq_cache = (seq_t *)calloc(seq_cap + 1, sizeof(seq_t));
        seq_cache[1] = 1;
        seq_len = 1;

        used_cap  = 32;
        used_flags = (unsigned char *)calloc(used_cap + 1, sizeof(unsigned char));
        used_flags[1] = 1;
        next_unseen = 2;
    }

    /* Extend sequence until it contains the requested index */
    while (seq_len < n) {
        idx_t k = (seq_len + 1) / 2;   /* current k */

        /* Find p(k): smallest unused integer */
        ensure_used_capacity(next_unseen);
        while (used_flags[(size_t)next_unseen]) {
            ++next_unseen;
            ensure_used_capacity(next_unseen);
        }
        seq_t p = (seq_t)next_unseen;
        used_flags[(size_t)next_unseen] = 1;   /* mark p as used */

        /* Compute q(k) = p + k and mark it as used */
        seq_t q = p + (seq_t)k;
        idx_t q_idx = (idx_t)q;
        ensure_used_capacity(q_idx);
        used_flags[(size_t)q_idx] = 1;        /* mark q as used */

        /* Store a(2k) and a(2k+1) */
        idx_t idx_a2 = 2 * k;
        idx_t idx_a3 = 2 * k + 1;
        ensure_cache_capacity(idx_a3);
        seq_cache[(size_t)idx_a2] = q;
        seq_cache[(size_t)idx_a3] = p;
        seq_len = (size_t)idx_a3;

        /* Advance next_unseen to next ID candidate */
        do {
            ++next_unseen;
            ensure_used_capacity(next_unseen);
        } while (used_flags[(size_t)next_unseen]);
    }

    return seq_cache[(size_t)n];
}
