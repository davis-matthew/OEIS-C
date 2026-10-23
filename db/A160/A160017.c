/*
 * A160017  Lodumo_2 of Thue-Morse sequence A010060.
 * https://oeis.org/A160017
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = lod_2(A010060(n)) Here A010060(n) is the Thue–Morse
 *            sequence (parity of popcount of n). lod_2 produces the k‑th evil
 *            number (even popcount) for even n and the k‑th odious number
 *            (odd popcount) for odd n. The function maintains a cache of evil
 *            and odious numbers, computed incrementally in O...
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *evils   = NULL;   /* array of evil numbers (popcount even) */
static seq_t *odious  = NULL;   /* array of odious numbers (popcount odd) */
static size_t evils_cap   = 0;
static size_t odious_cap  = 0;
static idx_t  evils_count  = 0;
static idx_t  odious_count = 0;
static idx_t  next_candidate = 0;   /* next integer to classify (starting at 0) */

/* Expand a dynamic array by doubling its capacity or initializing to 8. */
static void ensure_cap(size_t *cap, size_t *count, seq_t **arr)
{
    if (*count >= *cap) {
        size_t new_cap = (*cap == 0) ? 8 : (*cap << 1);
        seq_t *p = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        if (!p) abort();                     /* allocation failure */
        *arr = p;
        *cap = new_cap;
    }
}

/* Generate numbers until we have at least (need_evil + 1) evil numbers
   and (need_odious + 1) odious numbers.  Numbers are processed in
   increasing order. */
static void generate_up_to(idx_t need_evil, idx_t need_odious)
{
    while (evils_count <= need_evil || odious_count <= need_odious) {
        /* classify next_candidate */
        unsigned long long v = (unsigned long long)next_candidate;
        unsigned pc = __builtin_popcountll(v);
        if ((pc & 1) == 0) {          /* evil (even popcount) */
            ensure_cap(&evils_cap, &evils_count, &evils);
            evils[evils_count++] = (seq_t)v;
        } else {                      /* odious (odd popcount) */
            ensure_cap(&odious_cap, &odious_count, &odious);
            odious[odious_count++] = (seq_t)v;
        }
        next_candidate++;             /* move to next integer */
    }
}

seq_t a(idx_t n)
{
    if ((n & 1ULL) == 0) {            /* even n -> k‑th evil number */
        idx_t k = n >> 1;
        if (k >= evils_count)
            generate_up_to(k, 0);      /* generate up to k evil numbers */
        return evils[k];
    } else {                          /* odd n -> k‑th odious number */
        idx_t k = n >> 1;
        if (k >= odious_count)
            generate_up_to(0, k);      /* generate up to k odious numbers */
        return odious[k];
    }
}
