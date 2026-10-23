/*
 * A188848  Number of nX6 binary arrays without the pattern 0 0 1 vertically,
 *          antidiagonally or horizontally.
 * https://oeis.org/A188848
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the number of n×6 binary arrays without the pattern 0 0 1
 *            vertically, antidiagonally or horizontally. Computation is by
 *            dynamic programming over the last two rows -- a linear O(n)
 *            algorithm
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/* Tables for the 33 valid 6‑bit rows (no horizontal 001) and their
   transitions.  Each state is a pair of previous rows; transitions
   keep two previous rows and add a new row when no forbidden pattern
   appears. */
static unsigned char pat_to_idx[64];          /* pattern → index, 255 = invalid  */
static uint8_t     validIndices[33];          /* list of all valid 6‑bit rows   */
static unsigned char trans[1089][33];         /* transitions from a state      */
static unsigned char trans_len[1089];         /* length of each transition list*/
static unsigned char numValidRows = 0;        /* number of valid rows (33)      */
static int tables_init  = 0;

/* ------------------------------------------------------------------ */
static void init_tables(void)
{
    if (tables_init) return;
    tables_init = 1;

    /* mark all possible 6‑bit patterns as invalid */
    for (int i = 0; i < 64; ++i) pat_to_idx[i] = 255;

    /* collect rows that have no 001 horizontally */
    for (int pat = 0; pat < 64; ++pat) {
        int ok = 1;
        for (int k = 0; k <= 3; ++k)
            if (((pat >> k) & 7) == 4) { ok = 0; break; }
        if (ok) {
            pat_to_idx[pat] = numValidRows;
            validIndices[numValidRows++] = (uint8_t)pat;
        }
    }

    /* pre‑compute transitions for every pair of previous rows.
       States are indexed by (prev2Idx*33 + prev1Idx). */
    for (unsigned char p2i = 0; p2i < numValidRows; ++p2i)
        for (unsigned char p1i = 0; p1i < numValidRows; ++p1i) {
            int s = p2i * 33 + p1i;
            unsigned char len = 0;
            uint8_t prev2 = validIndices[p2i];
            uint8_t prev1 = validIndices[p1i];
            for (unsigned char ci = 0; ci < numValidRows; ++ci) {
                uint8_t cur = validIndices[ci];
                int ok = 1;

                /* vertical 0 0 1 check */
                for (int k = 0; k < 6 && ok; ++k)
                    if (!( (prev2 >> k) & 1 )
                     && !( (prev1 >> k) & 1 )
                     &&  ( (cur   >> k) & 1 ))
                        ok = 0;

                /* antidiagonal 0 0 1 check */
                for (int k = 0; k < 4 && ok; ++k)
                    if (!( (prev2 >> (k+2)) & 1 )
                     && !( (prev1 >> (k+1)) & 1 )
                     &&  ( (cur   >> k) & 1 ))
                        ok = 0;

                if (ok) trans[s][len++] = ci;
            }
            trans_len[s] = len;
        }
}

/* ------------------------------------------------------------------ */
/* DP storage: 1089 states (33^2) for the pair of most recent rows */
static seq_t dp_cur[1089];
static seq_t dp_next[1089];

/* ------------------------------------------------------------------ */
/* Cached values of a(n) for fast repeated or out‑of‑order queries */
static seq_t *a_cache = NULL;     /* array of cached a(n) values */
static size_t cache_cap = 0;      /* current capacity of a_cache    */
static idx_t  maxComputed = 0;    /* largest n for which a(n) is stored */

/* ------------------------------------------------------------------ */
/* Ensure cache can hold values up to index n (inclusive).  Works for
   both C and C++ (the latter constructs elements when using new[]). */
static void ensure_cache(idx_t n)
{
    if ((size_t)(n + 1) <= cache_cap) return;
    size_t new_cap = (size_t)(n + 1);
    seq_t *new_ptr;
#ifdef __cplusplus
    new_ptr = new seq_t[new_cap];
    if (a_cache) {
        for (size_t i = 0; i < cache_cap; ++i) new_ptr[i] = a_cache[i];
        delete[] a_cache;
    }
#else
    new_ptr = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
#endif
    a_cache = new_ptr;
    cache_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/* Main entry function requested by the problem statement */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset 1: a(1) is the first term */

    init_tables();
    ensure_cache(n);

    /* compute values up to n if not already cached */
    while (maxComputed < n) {
        if (maxComputed == 0) {                    /* a(1) */
            a_cache[1] = numValidRows;             /* 33 */
            maxComputed = 1;
            continue;
        }

        if (maxComputed == 1) {                    /* a(2) */
            a_cache[2] = (seq_t)numValidRows * (seq_t)numValidRows; /* 1089 */
            for (size_t i = 0; i < 1089; ++i) dp_cur[i] = (seq_t)1;
            maxComputed = 2;
            continue;
        }

        /* step for n >= 3 : one DP transition */
        for (int i = 0; i < 1089; ++i) dp_next[i] = (seq_t)0;

        for (int s = 0; s < 1089; ++s) {
            seq_t ways = dp_cur[s];
            if (!ways) continue;
            unsigned char prev1Idx = s % 33;
            unsigned char *list = trans[s];
            unsigned char len   = trans_len[s];
            for (unsigned char t = 0; t < len; ++t) {
                int dest = prev1Idx * 33 + list[t];
                dp_next[dest] += ways;
            }
        }

        /* copy dp_next into dp_cur element‑wise (avoid shallow copy) */
        for (int i = 0; i < 1089; ++i) dp_cur[i] = dp_next[i];

        /* sum all states to obtain a(n) */
        seq_t total = (seq_t)0;
        for (int i = 0; i < 1089; ++i) total += dp_cur[i];
        a_cache[++maxComputed] = total;
    }

    return a_cache[n];
}
