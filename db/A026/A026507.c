/*
 * A026507  a(n) = least positive integer > a(n-1) and not (a(i)+a(j))^2 -
 *          a(k)^2 for 1<=i<j<k<=n.
 * https://oeis.org/A026507
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   bitset_size, u32_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A026507: a(n) = least positive integer > a(n‑1) that is not of the
 * form (a(i)+a(j))^2 – a(k)^2 with 1 ≤ i < j < k ≤ n.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/bitset_size.h"
#include "../utils/u32_sqrt.h"

/* ---------- Parameters ---------- */
#define LIMIT 40000U          /* maximum value that must be tracked as “bad” */
                                 /* (fits in any seq_t chosen by the build) */

static inline bool is_bad(idx_t v, const uint8_t *bits)
{
    if ((idx_t)v > LIMIT) return false;
    return (bits[(idx_t)v >> 3] >> ((idx_t)v & 7)) & 1U;
}

static inline void set_bad(idx_t v, uint8_t *bits)
{
    if ((idx_t)v > LIMIT) return;
    bits[(idx_t)v >> 3] |= 1U << ((idx_t)v & 7);
}

/* ---------- Static storage ---------- */
static seq_t *seq      = NULL;   /* generated sequence values */
static idx_t   seq_len = 0;        /* number of values stored */
static idx_t   seq_cap = 0;        /* capacity of seq buffer */
static uint8_t *bad_bits = NULL;   /* bitset of “bad” numbers */

/* ---------- Helpers to resize arrays ---------- */
static void grow_seq(void)
{
    if (seq_len >= seq_cap) {
        seq_cap = (seq_cap == 0) ? 16 : seq_cap * 2;
        seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
    }
}

static void init_storage(void)
{
    if (seq == NULL) {
        seq_cap = 16;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        seq_len = 0;
    }

    if (bad_bits == NULL)
        bad_bits = (uint8_t *)calloc(bitset_size(LIMIT), 1);

    /* initialise with a(1)=1 if first call */
    if (seq_len == 0) {
        grow_seq();
        seq[0] = 1;
        seq_len = 1;
    }
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* undefined for n==0, defined behaviour only for n≥1 */

    init_storage();

    while (seq_len < n) {               /* extend sequence up to requested index (1‑based) */
        /* find smallest candidate > last value and not “bad” */
        seq_t cand = seq[seq_len - 1] + 1;
        while (is_bad((idx_t)cand, bad_bits)) ++cand;

        grow_seq();
        seq[seq_len] = cand;
        seq_len++;

        /* generate new forbidden numbers involving this term, if k≥3 */
        idx_t idx = seq_len - 1;  /* zero‑based index of the newly added term */
        if (idx < 2) continue;    /* no pairs i<j<k available yet */

        seq_t ak = seq[idx];
        /* compute upper bound for sums that can produce a forbidden value ≤ LIMIT */
        uint32_t ak_u = (uint32_t)ak;
        uint32_t sq   = ak_u * ak_u + LIMIT;
        uint32_t limX_u = u32_sqrt(sq);
        seq_t limX = (seq_t)limX_u;

        for (idx_t i = 0; i < idx - 1; ++i) {
            for (idx_t j = i + 1; j < idx; ++j) {
                seq_t p = seq[i] + seq[j];
                if (p <= ak) continue;                              /* val ≤ 0, ignore */
                if ((uint32_t)p > limX_u) break;                    /* further j will grow p */
                /* val = (p - ak) * (p + ak) */
                seq_t diff  = p - ak;
                seq_t sum   = p + ak;
                seq_t val   = diff * sum;                           /* fits in 32‑bit */
                if (val <= LIMIT) set_bad((idx_t)val, bad_bits);
            }
        }
    }

    return seq[(size_t)n - 1];
}
