/*
 * A188849  Number of n X 7 binary arrays without the pattern 0 0 1
 *          vertically, antidiagonally or horizontally.
 * https://oeis.org/A188849
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Implements OEIS sequence A188849:
 * number of n × 7 binary arrays that avoid the pattern 0 0 1 vertically,
 * antidiagonally or horizontally.
 * Uses dynamic programming over consecutive row pairs; the adjacency
 * information is encoded in 64‑bit masks to keep the transition fast.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* Sequencer type definitions are supplied by the build; we do NOT define them here. */
/* idx_t  – unsigned integer index type (e.g., uint64_t) */
/* seq_t  – unsigned integer value type (e.g., uint64_t, unsigned __int128, big integer class) */

/* ---------- Precomputation data ---------------------------------------- */
static bool initialized = false;

/* Valid 7‑bit rows (no horizontal 001).  There are 54 of them. */
#define MAX_ROWS 128
static uint8_t rows[54];
static size_t row_count = 0;

/* For every ordered pair (i,j) the set of third rows k that keep the
 * triple (rows[i],rows[j],rows[k]) valid.  Stored as a 64‑bit mask
 * (k-th bit corresponds to rows[k]). */
static uint64_t *adjacency_masks = NULL;   /* size row_count * row_count */

/* DP tables for the last two rows.  Flat arrays of size row_count*row_count. */
static seq_t *dp_cur = NULL;              /* current state (length L) */
static seq_t *dp_next = NULL;             /* next state (length L+1) */
static size_t dp_size = 0;                /* row_count * row_count */

/* Cache of a(n) values.  cache[0] unused; cache[1],cache[2],... */
static seq_t *cache = NULL;
static size_t cache_len = 0;              /* highest n already cached (>=2) */

/* Current length represented by dp_cur (>=2). */
static uint64_t dp_len = 0;

/* ------------------------------------------------------------------------ */
static inline bool triple_valid(uint8_t a, uint8_t b, uint8_t c)
{
    /* vertical 0 0 1: any column where a=0,b=0,c=1 is forbidden */
    if ((~a & ~b & c) != 0) return false;

    /* antidiagonal 0 0 1: for columns 2..6 */
    for (int j = 2; j < 7; ++j) {
        uint8_t bit_a = (a >> j) & 1U;
        uint8_t bit_b = (b >> (j - 1)) & 1U;
        uint8_t bit_c = (c >> (j - 2)) & 1U;
        if (bit_a == 0 && bit_b == 0 && bit_c == 1) return false;
    }
    return true;
}

/* ------------------------------------------------------------------------ */
static void init(void)
{
    if (initialized) return;

    /* ---- All admissible rows (no horizontal 001) ---- */
    for (uint16_t v = 0; v < MAX_ROWS; ++v) {
        bool ok = true;
        for (int j = 0; j <= 4; ++j) {
            uint8_t b0 = (v >> j) & 1U;
            uint8_t b1 = (v >> (j + 1)) & 1U;
            uint8_t b2 = (v >> (j + 2)) & 1U;
            if (b0 == 0 && b1 == 0 && b2 == 1) { ok = false; break; }
        }
        if (ok) rows[row_count++] = (uint8_t)v;
    }

    /* ---- Adjacency masks for triple validity ---- */
    dp_size = row_count * row_count;
    adjacency_masks = (uint64_t*)malloc(dp_size * sizeof(uint64_t));
    for (size_t i = 0; i < row_count; ++i) {
        for (size_t j = 0; j < row_count; ++j) {
            uint64_t mask = 0;
            for (size_t k = 0; k < row_count; ++k) {
                if (triple_valid(rows[i], rows[j], rows[k]))
                    mask |= ((uint64_t)1U << k);
            }
            adjacency_masks[i * row_count + j] = mask;
        }
    }

    /* ---- DP tables ---- */
    dp_cur  = (seq_t*) calloc(dp_size, sizeof(seq_t));
    dp_next = (seq_t*) calloc(dp_size, sizeof(seq_t));
    /* length 2: all pairs allowed, count 1 each */
    for (size_t p = 0; p < dp_size; ++p) dp_cur[p] = (seq_t)1;
    dp_len = 2;

    /* ---- Cache: a(1) and a(2) ---- */
    cache_len = 2;
    cache = (seq_t*) malloc((cache_len + 1) * sizeof(seq_t));
    cache[0] = (seq_t)0;                       /* unused */
    cache[1] = (seq_t)row_count;               /* a(1) = number of rows */
    cache[2] = (seq_t)row_count * (seq_t)row_count; /* a(2) = rows^2 */

    initialized = true;
}

/* ------------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    init();

    if (n <= cache_len) return cache[n];

    /* Extend cache and DP up to n */
    for (idx_t len = cache_len + 1; len <= n; ++len) {
        /* Transition from dp_cur (length=len-1) to dp_next (length=len) */
        memset(dp_next, 0, dp_size * sizeof(seq_t));

        for (size_t i = 0; i < row_count; ++i) {
            size_t base_i = i * row_count;
            for (size_t j = 0; j < row_count; ++j) {
                seq_t count = dp_cur[base_i + j];
                if (count == (seq_t)0) continue;

                uint64_t mask = adjacency_masks[base_i + j];
                while (mask) {
                    int k = __builtin_ctzll(mask);
                    mask &= mask - 1;
                    dp_next[j * row_count + k] += count;
                }
            }
        }

        /* Sum all entries of dp_next → a(len) */
        seq_t sum = (seq_t)0;
        for (size_t p = 0; p < dp_size; ++p) sum += dp_next[p];
        /* Increase cache size if necessary */
        if ((size_t)len > cache_len) {
            cache = (seq_t*)realloc(cache, (len + 1) * sizeof(seq_t));
            cache_len = len;
        }
        cache[len] = sum;

        /* Prepare for next iteration */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
        dp_len = len;
    }

    return cache[n];
}
