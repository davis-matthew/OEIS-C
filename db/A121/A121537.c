/*
 * A121537  If n appears then 2n, 3n and 4n do not.
 * https://oeis.org/A121537
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A121537: if n appears then 2n, 3n and 4n are excluded. */
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/* Static memoisation state ------------------------------------------------- */
static seq_t *seq_cache = NULL;      /* stored terms           */
static size_t  seq_cap  = 0;         /* cache capacity        */
static size_t  seq_count = 0;        /* number of stored terms */

static uint8_t *banned = NULL;       /* 1‑based forbidden flags */
static size_t  ban_cap  = 0;         /* capacity of banned array */

/* Current smallest integer that has not been processed yet. */
static idx_t   current_num = 1;

/* Ensure seq_cache can hold at least `need` elements. */
static void ensure_seq_cap(size_t need)
{
    if (need <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap : 64;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);
    seq_cache = tmp;
    seq_cap   = new_cap;
}

/* Ensure the banned array can be indexed at least up to `need`. */
static void ensure_banned_cap(idx_t need)
{
    if (banned && (size_t)need < ban_cap) return;
    size_t old_cap = ban_cap;
    size_t new_cap = ban_cap ? ban_cap : 1;
    while (new_cap <= (size_t)need) new_cap <<= 1;
    uint8_t *tmp = (uint8_t *)realloc(banned, new_cap * sizeof(uint8_t));
    if (!tmp) exit(1);
    /* Zero the newly allocated portion */
    memset(tmp + old_cap, 0, new_cap - old_cap);
    banned   = tmp;
    ban_cap  = new_cap;
}

/* Return the n‑th term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset starts at 1 */

    /* Generate terms until we have at least n of them. */
    while (seq_count < (size_t)n) {
        /* Make sure we can examine current_num. */
        ensure_banned_cap(current_num);

        /* Skip numbers already forbidden. */
        while (current_num <= ban_cap && banned[current_num]) {
            ++current_num;
            ensure_banned_cap(current_num);
        }

        /* current_num is now a new valid term */
        ensure_seq_cap(seq_count + 1);
        seq_cache[seq_count++] = (seq_t)current_num;

        /* Mark its multiples 2, 3 and 4 as forbidden. */
        for (idx_t k = 2; k <= 4; ++k) {
            idx_t m = (idx_t)k * current_num;
            ensure_banned_cap(m);
            banned[m] = 1;
        }

        /* Advance to the next candidate. */
        ++current_num;
    }

    return seq_cache[n - 1];
}
