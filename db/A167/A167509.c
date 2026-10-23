/*
 * A167509  Least positive integer written with n different letters when
 *          spelled out in French.
 * https://oeis.org/A167509
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  fini, nonn, word
 * Formula:   a(n) = min { k | A167508(k) = n } (minimal integer whose French
 *            spelling uses exactly n distinct letters)
 * Range:     u8 n <= 14, u16 n <= 14, u32 n <= 14, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   word_mask
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/word_mask.h"

/* Masks for atomic French words */
static uint32_t mask_un, mask_deux, mask_trois, mask_quatre, mask_cinq, mask_six, mask_sept,
                 mask_huit, mask_neuf, mask_dix, mask_onze, mask_douze, mask_treize,
                 mask_quatorze, mask_quinze, mask_seize, mask_et, mask_cent, mask_cents,
                 mask_vingt, mask_trente, mask_quarante, mask_cinquante, mask_soixante,
                 mask_quatrevingt, mask_quatrevingts, mask_mille, mask_million,
                 mask_milliard;

/* Initialize the masks once */
__attribute__((constructor))
static void init_masks(void)
{
    mask_un          = word_mask("un");
    mask_deux        = word_mask("deux");
    mask_trois       = word_mask("trois");
    mask_quatre      = word_mask("quatre");
    mask_cinq        = word_mask("cinq");
    mask_six         = word_mask("six");
    mask_sept        = word_mask("sept");
    mask_huit        = word_mask("huit");
    mask_neuf        = word_mask("neuf");
    mask_dix         = word_mask("dix");
    mask_onze        = word_mask("onze");
    mask_douze       = word_mask("douze");
    mask_treize      = word_mask("treize");
    mask_quatorze    = word_mask("quatorze");
    mask_quinze      = word_mask("quinze");
    mask_seize       = word_mask("seize");
    mask_et          = word_mask("et");
    mask_cent        = word_mask("cent");
    mask_cents       = word_mask("cents");
    mask_vingt       = word_mask("vingt");
    mask_trente      = word_mask("trente");
    mask_quarante    = word_mask("quarante");
    mask_cinquante   = word_mask("cinquante");
    mask_soixante    = word_mask("soixante");
    mask_quatrevingt = word_mask("quatrevingt");
    mask_quatrevingts= word_mask("quatrevingts");
    mask_mille       = word_mask("mille");
    mask_million     = word_mask("million");
    mask_milliard    = word_mask("milliard");
}

/* Masks for single digits */
static inline uint32_t mask_unit(int d)
{
    switch (d) {
    case 1: return mask_un;
    case 2: return mask_deux;
    case 3: return mask_trois;
    case 4: return mask_quatre;
    case 5: return mask_cinq;
    case 6: return mask_six;
    case 7: return mask_sept;
    case 8: return mask_huit;
    case 9: return mask_neuf;
    default: return 0;
    }
}

/* Masks for 10–16 */
static inline uint32_t mask_10_to_16(int d)
{
    switch (d) {
    case 10: return mask_dix;
    case 11: return mask_onze;
    case 12: return mask_douze;
    case 13: return mask_treize;
    case 14: return mask_quatorze;
    case 15: return mask_quinze;
    case 16: return mask_seize;
    default: return 0;
    }
}

/* Masks for 12–19 used only in the special 70–79 and 90–99 ranges */
static inline uint32_t mask_12_to_19(int d)
{
    switch (d) {
    case 12: return mask_douze;
    case 13: return mask_treize;
    case 14: return mask_quatorze;
    case 15: return mask_quinze;
    case 16: return mask_seize;
    case 17: return mask_dix | mask_sept;
    case 18: return mask_dix | mask_huit;
    case 19: return mask_dix | mask_neuf;
    default: return 0;
    }
}

/* Mask for numbers 0–99 */
static uint32_t mask_0_to_99(int n)
{
    uint32_t m = 0;
    if (n == 0) return 0;

    /* 0–9 */
    if (n < 10) return mask_unit(n);

    /* 10–16 */
    if (n < 17) return mask_10_to_16(n);

    /* 17–19 */
    if (n < 20) return mask_dix | mask_unit(n - 10);

    /* 20–69 */
    if (n < 70) {
        int ten = n / 10;     /* 2..6 */
        int unit = n % 10;
        switch (ten) {
        case 2: m = mask_vingt; break;
        case 3: m = mask_trente; break;
        case 4: m = mask_quarante; break;
        case 5: m = mask_cinquante; break;
        case 6: m = mask_soixante; break;
        }
        if (unit == 1) {                 /* 21,31,41,51,61 */
            m |= mask_et | mask_unit(1);
        } else if (unit > 0) {
            m |= mask_unit(unit);
        }
        return m;
    }

    /* 70–79 */
    if (n < 80) {
        switch (n) {
        case 70: m = mask_soixante | mask_dix; break;
        case 71: m = mask_soixante | mask_et | mask_onze; break;
        case 72: m = mask_soixante | mask_douze; break;
        case 73: m = mask_soixante | mask_treize; break;
        case 74: m = mask_soixante | mask_quatorze; break;
        case 75: m = mask_soixante | mask_quinze; break;
        case 76: m = mask_soixante | mask_seize; break;
        case 77: m = mask_soixante | mask_dix | mask_sept; break;
        case 78: m = mask_soixante | mask_dix | mask_huit; break;
        case 79: m = mask_soixante | mask_dix | mask_neuf; break;
        }
        return m;
    }

    /* 80–99 */
    if (n < 100) {
        switch (n) {
        case 80: m = mask_quatrevingts; break;
        case 81: m = mask_quatrevingt | mask_et | mask_un; break;
        case 90: m = mask_quatrevingt | mask_dix; break;
        case 91: m = mask_quatrevingt | mask_onze; break;
        case 92: m = mask_quatrevingt | mask_douze; break;
        case 93: m = mask_quatrevingt | mask_treize; break;
        case 94: m = mask_quatrevingt | mask_quatorze; break;
        case 95: m = mask_quatrevingt | mask_quinze; break;
        case 96: m = mask_quatrevingt | mask_seize; break;
        case 97: m = mask_quatrevingt | mask_dix | mask_sept; break;
        case 98: m = mask_quatrevingt | mask_dix | mask_huit; break;
        case 99: m = mask_quatrevingt | mask_dix | mask_neuf; break;
        default:
            /* 82–89 */
            m = mask_quatrevingt | mask_unit(n % 10);
            break;
        }
        return m;
    }
    return m;  /* unreachable */
}

/* Mask for numbers 1–999 */
static uint32_t mask_1_to_999(int n)
{
    uint32_t m = 0;
    if (n == 0) return 0;
    int h = n / 100;
    int rem = n % 100;
    if (h > 0) {
        if (h == 1) {
            m |= mask_cent;
        } else {
            m |= mask_1_to_999(h);
            if (rem == 0) {
                m |= mask_cents;
            } else {
                m |= mask_cent;
            }
        }
    }
    if (rem > 0) {
        m |= mask_0_to_99(rem);
    }
    return m;
}

/* Mask for any positive integer */
static uint32_t french_mask(idx_t n)
{
    uint32_t m = 0;
    if (n >= 1000000000ULL) {                   /* billions */
        idx_t b = n / 1000000000ULL;
        n %= 1000000000ULL;
        m |= mask_1_to_999((int)b);
        m |= mask_milliard;
    }
    if (n >= 1000000ULL) {                       /* millions */
        idx_t mll = n / 1000000ULL;
        n %= 1000000ULL;
        m |= mask_1_to_999((int)mll);
        m |= mask_million;                       /* plural form ignored in mask */
    }
    if (n >= 1000ULL) {                           /* thousands */
        idx_t th = n / 1000ULL;
        n %= 1000ULL;
        m |= mask_1_to_999((int)th);
        m |= mask_mille;
    }
    if (n > 0) {                                  /* remainder 1–999 */
        m |= mask_1_to_999((int)n);
    }
    return m;
}

/* Cache for minimal numbers for each distinct‑letter count */
#define MAX_D 30
static seq_t best[MAX_D];
static bool has_best[MAX_D];
static idx_t _next = 1;
static idx_t _max_needed = 0;

/* Ensure cache filled up to required n */
static void fill_cache(idx_t need)
{
    if (need < 2 || need >= MAX_D) return;
    if (need <= _max_needed) return;
    _max_needed = need;
    while (true) {
        uint32_t m = french_mask(_next);
        int d = __builtin_popcount(m);
        if (d < MAX_D && !has_best[d]) {
            best[d] = (seq_t)_next;
            has_best[d] = true;
        }
        bool all = true;
        for (idx_t i = 2; i <= _max_needed; ++i)
            if (!has_best[i]) { all = false; break; }
        if (all) break;
        ++_next;
    }
}

/* --------------------------------------------------------------- */
/*   Public sequence function
 *   seq_t a(idx_t n)
 * --------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 2 || n >= MAX_D) return 0;
    fill_cache(n);
    return best[n];
}
