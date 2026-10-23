/*
 * A161145  Numbers which can be expressed as the product of numbers made of
 *          only sevens.
 * https://oeis.org/A161145
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   numbers expressible as product of numbers comprised only of 7's
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 130, u64 n <= 2557,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

/*-----------------------------------------------------------
   Helper to multiply two seq_t values with overflow detection.
   Works for small sizes (≤ 8 bytes) by using unsigned __int128.
   For larger seq_t types (arbitrary‑precision) the multiplication
   is performed directly.
  -----------------------------------------------------------*/
static inline int mul_overflow(seq_t a, seq_t b, seq_t *res)
{
    if (sizeof(seq_t) <= 8) {                     /* 64‑bit or smaller */
#if defined(__SIZEOF_INT128__)
        unsigned __int128 tmp = (unsigned __int128)a * (unsigned __int128)b;
        if (tmp > (unsigned __int128)UINT64_MAX) {
            return 1;           /* overflow */
        }
        *res = (seq_t)tmp;
        return 0;
#else
        /* No 128-bit support: assume no overflow for 64‑bit builds */
        *res = a * b;
        return 0;
#endif
    } else {
        *res = a * b;           /* big‑int: relies on operator* */
        return 0;
    }
}

/* Dynamic array append helper */
static void seq_append(seq_t **arr, size_t *cap, size_t *len, seq_t val)
{
    if (*len == *cap) {
        *cap = (*cap == 0) ? 16 : (*cap << 1);
        *arr = (seq_t *)realloc(*arr, (*cap) * sizeof(seq_t));
    }
    (*arr)[*len] = val;
    (*len)++;
}

/* ----------------------------------------------------------- */
static seq_t *SEQ      = NULL;      /* sequence values */
static size_t  SEQ_cap = 0;
static size_t  SEQ_len = 0;

static seq_t *GEN      = NULL;      /* digit‑seven generators 7,77,777,... */
static size_t  GEN_len = 0;
static size_t *IDX     = NULL;      /* indices into SEQ for each generator */

static int INIT_DONE = 0;

/* ----------------------------------------------------------- */
static void init_sequence(void)
{
    /* Create the list of generators up to a generous length (50 digits) */
    const size_t MAX_GENS = 50;
    GEN_len = 0;
    GEN = (seq_t *)malloc(MAX_GENS * sizeof(seq_t));
    seq_t repunit = 0;
    for (size_t len = 1; len <= MAX_GENS; ++len) {
        seq_t tmp;
        /* repunit = repunit*10 + 1  (creates 1, 11, 111, ...) */
        if (mul_overflow(repunit, (seq_t)10, &tmp)) break;   /* overflow for small types */
        repunit = tmp + (seq_t)1;
        /* g = repunit * 7  (7, 77, 777, ...) */
        if (mul_overflow(repunit, (seq_t)7, &tmp)) break;
        GEN[GEN_len++] = tmp;
    }

    IDX = (size_t *)calloc(GEN_len, sizeof(size_t));

    SEQ_cap = 16;
    SEQ_len = 1;
    SEQ = (seq_t *)malloc(SEQ_cap * sizeof(seq_t));
    SEQ[0] = (seq_t)1;   /* a(1) = 1 */
}

/* ----------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (!INIT_DONE) {
        init_sequence();
        INIT_DONE = 1;
    }

    while (SEQ_len < (size_t)n) {
        seq_t min_val = 0;
        int first = 1;

        /* Find smallest candidate among g * SEQ[IDX[g]] */
        for (size_t i = 0; i < GEN_len; ++i) {
            seq_t cand;
            if (mul_overflow(GEN[i], SEQ[IDX[i]], &cand)) continue;  /* overflow -> ignore */
            if (first || cand < min_val) {
                min_val = cand;
                first = 0;
            }
        }

        /* Append the minimal value */
        seq_append(&SEQ, &SEQ_cap, &SEQ_len, min_val);

        /* Increment indices for generators that produced min_val */
        for (size_t i = 0; i < GEN_len; ++i) {
            seq_t cand;
            if (mul_overflow(GEN[i], SEQ[IDX[i]], &cand)) continue;
            if (cand == min_val) IDX[i]++;
        }
    }

    return SEQ[n-1];
}
