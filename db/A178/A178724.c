/*
 * A178724  Nonnegative integers whose English name has at least one letter in
 *          common with the English name of all other integers.
 * https://oeis.org/A178724
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word
 * Formula:   a(n) = the n‑th nonnegative integer whose English name shares at
 *            least one letter with the English name of every other integer
 *            (property described in OEIS A178724)
 * Range:     u8 n <= 51, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   mask_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/mask_of.h"

/* --------------------------------------------------------------------- */

/* --------------------------------------------------------------------- */
/*  Fixed data: names for numbers < 20 and the tens                         */
static const char *w0_19[20] = {
    "zero","one","two","three","four","five","six","seven","eight","nine",
    "ten","eleven","twelve","thirteen","fourteen","fifteen","sixteen",
    "seventeen","eighteen","nineteen"
};

static const char *wtens[8] = {
    "twenty","thirty","forty","fifty","sixty","seventy","eighty","ninety"
};

/* --------------------------------------------------------------------- */
/*  Masks for 0..999                                                    */
#define UNIT_MAX 1000
static uint32_t unit_masks[UNIT_MAX];          /* mask for each 0..999 */
static uint32_t mask_hundred;

/*  Compute mask for a number < 1000                                    */
static void init_unit_masks(void)
{
    mask_hundred = mask_of("hundred");
    for (int n = 0; n < UNIT_MAX; ++n) {
        if (n < 20) {
            unit_masks[n] = mask_of(w0_19[n]);
        } else if (n < 100) {
            int tens = n / 10;
            int u = n % 10;
            uint32_t m = mask_of(wtens[tens - 2]);        /* 20 -> 0 */
            if (u) m |= mask_of(w0_19[u]);
            unit_masks[n] = m;
        } else { /* 100..999 */
            int h = n / 100;
            int rest = n % 100;
            uint32_t m = mask_of(w0_19[h]) | mask_hundred;
            if (rest) {
                if (rest < 20) {
                    m |= mask_of(w0_19[rest]);
                } else {
                    int tens = rest / 10;
                    int u = rest % 10;
                    m |= mask_of(wtens[tens - 2]);
                    if (u) m |= mask_of(w0_19[u]);
                }
            }
            unit_masks[n] = m;
        }
    }
}

/* --------------------------------------------------------------------- */
/*  Magnitude words and masks                                           */
#define GROUP_MAX 6   /* thousand^1 ... quintillion^6 */
static uint32_t magnitude_masks[GROUP_MAX + 1];   /* index 0 unused */

/*  Masks for the group values (1..999) including the magnitude word */
#define MAX_GVAL 999
static uint32_t group_masks[GROUP_MAX + 1][MAX_GVAL]; /* [group][value-1] */

/*  Initialization of magnitude masks and group masks                   */
static void init_magnitudes(void)
{
    static const char *magnitude_names[] = {
        "",          /* 0 unused */
        "thousand",
        "million",
        "billion",
        "trillion",
        "quadrillion",
        "quintillion"
    };
    for (int g = 0; g <= GROUP_MAX; ++g)
        magnitude_masks[g] = mask_of(magnitude_names[g]);

    /* compute group masks: unit mask OR magnitude mask, for all v=1..999 */
    for (int g = 1; g <= GROUP_MAX; ++g) {
        for (int v = 1; v <= MAX_GVAL; ++v) {
            group_masks[g][v - 1] = unit_masks[v] | magnitude_masks[g];
        }
    }
}

/* --------------------------------------------------------------------- */
/*  Bitset for seen masks – uses 26 bits (letters a..z)                 */
#define MAX_MASK_BITS (1 << 26)
static unsigned char *seen_bitset = NULL;       /* 2^26 bits ~ 8 MB */

/*  Bitset helpers */
static inline bool seen_is(uint32_t m)
{
    return (seen_bitset[m >> 3] >> (m & 7)) & 1u;
}
static inline void seen_set(uint32_t m)
{
    seen_bitset[m >> 3] |= 1u << (m & 7);
}

/* --------------------------------------------------------------------- */
/*  Dynamic vector of uint32_t                                            */
typedef struct {
    uint32_t *data;
    size_t   len;
    size_t   cap;
} vec_uint32;

static void vec_init(vec_uint32 *v, size_t cap)
{
    v->len = 0;
    v->cap = cap;
    v->data = (uint32_t *)malloc(cap * sizeof(uint32_t));
}
static void vec_push(vec_uint32 *v, uint32_t val)
{
    if (v->len == v->cap) {
        v->cap <<= 1;
        v->data = (uint32_t *)realloc(v->data, v->cap * sizeof(uint32_t));
    }
    v->data[v->len++] = val;
}
static void vec_free(vec_uint32 *v)
{
    free(v->data);
    v->len = v->cap = 0;
}

/* --------------------------------------------------------------------- */
/*  Build the complete set of letter‑sets of all numbers                */
static void build_all_masks(vec_uint32 *all_masks)
{
    seen_bitset = (unsigned char *)malloc(MAX_MASK_BITS / 8);   /* 8 MB */
    memset(seen_bitset, 0, MAX_MASK_BITS / 8);

    /* BFS queue */
    vec_uint32 queue;
    vec_init(&queue, 128);

    /* mark empty mask as seen but do NOT add it to the result set */
    seen_set(0);
    vec_push(&queue, 0);

    while (queue.len) {
        uint32_t m = queue.data[queue.len - 1];
        --queue.len;   /* pop */

        /* unit group (1..999) */
        for (int v = 1; v <= MAX_GVAL; ++v) {
            uint32_t nm = m | unit_masks[v];
            if (!seen_is(nm)) {
                seen_set(nm);
                vec_push(all_masks, nm);
                vec_push(&queue, nm);
            }
        }

        /* higher magnitude groups */
        for (int g = 1; g <= GROUP_MAX; ++g) {
            for (int v = 1; v <= MAX_GVAL; ++v) {
                uint32_t nm = m | group_masks[g][v - 1];
                if (!seen_is(nm)) {
                    seen_set(nm);
                    vec_push(all_masks, nm);
                    vec_push(&queue, nm);
                }
            }
        }
    }

    /* finally add the mask for the number zero ("zero") */
    uint32_t zero_mask = mask_of("zero");
    if (!seen_is(zero_mask)) {
        seen_set(zero_mask);
        vec_push(all_masks, zero_mask);
    }

    vec_free(&queue);
}

/* --------------------------------------------------------------------- */
/*  Compute the letter mask for an arbitrary index n (idx_t)              */
static uint32_t mask_from_idx(idx_t n)
{
    if (n == 0) return mask_of("zero");

    uint32_t m = 0;
    int pos = 0;
    while (n > 0) {
        uint32_t g = n % 1000;
        if (g) {
            m |= unit_masks[g];
            if (pos > 0)
                m |= magnitude_masks[pos];
        }
        n /= 1000;
        ++pos;
    }
    return m;
}

/* --------------------------------------------------------------------- */
/*  Global data                                                 */
static vec_uint32 all_masks_vec;
static bool all_masks_built = false;

/* --------------------------------------------------------------------- */
/*  Main sequence function                                            */
seq_t a(idx_t n)
{
    /* initialise once */
    if (!all_masks_built) {
        vec_init(&all_masks_vec, 8192);
        init_unit_masks();
        init_magnitudes();
        build_all_masks(&all_masks_vec);
        all_masks_built = true;
    }

    idx_t found = 0;
    idx_t count = 0;

    /* search for the n-th term */
    while (1) {
        uint32_t cand_mask = mask_from_idx(found);

        /* test property */
        bool ok = true;
        size_t sz = all_masks_vec.len;
        for (size_t i = 0; i < sz; ++i) {
            if ((cand_mask & all_masks_vec.data[i]) == 0) {
                ok = false;
                break;
            }
        }
        if (ok) {
            ++count;
            if (count == n)
                return (seq_t)found;
        }
        ++found;
    }
}
