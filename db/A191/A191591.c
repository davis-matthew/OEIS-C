/*
 * A191591  Number of different vowels in the Filipino (Tagalog) name of n.
 * https://oeis.org/A191591
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, word, easy
 * Formula:   not stated
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n): number of distinct vowels in the Filipino (Tagalog) name of n.
   Uses bit masks for vowel sets; running time is O(number of words). */

#include <stdint.h>

static const unsigned UNIT_MASK[10] = {            /* 0..9 */
    1,    /* wala   (a)            */
    5,    /* isa    (a,i)           */
    1,    /* dalawa (a)             */
    9,    /* tatlo  (a,o)           */
    1,    /* apat   (a)             */
    5,    /* lima   (a,i)           */
    5,    /* anim   (a,i)           */
    12,   /* pito   (i,o)           */
    9,    /* walo   (a,o)           */
    5     /* siyam  (a,i)           */
};

static const unsigned LABING_MASK = 5;   /* labing (a,i) */
static const unsigned SAMPU_MASK  = 17;  /* sampu  (a,u) */

static const unsigned TENS_MASK[8] = {    /* 20,30,...,90 */
    17,  /* dalawampu (a,u)       */
    25,  /* tatlumpu  (a,u,o)     */
    17,  /* apatnapu  (a,u)       */
    21,  /* limampu   (a,i,u)     */
    21,  /* animnapu  (a,i,u)     */
    28,  /* pitumpu   (i,o,u)     */
    25,  /* walumpu   (a,o,u)     */
    21   /* siyamnapu (a,i,u)     */
};

static const unsigned HUNDRED_MASK[10] = { /* 100,200,...,900 */
    0,   /* unused */
    5,   /* isip + daan  (a,i) */
    1,   /* dalawa + daan (a) */
    9,   /* tatlo + daan  (a,o) */
    1,   /* apat + daan   (a) */
    5,   /* lima + daan   (a,i) */
    5,   /* anim + daan   (a,i) */
    13,  /* pito + daan   (a,i,o) */
    9,   /* walo + daan   (a,o) */
    5    /* siyam + daan  (a,i) */
};

static const unsigned THOUSAND_MASK[11] = { /* 1,000 – 10,000 */
    0,   /* unused */
    13,  /* isang libo           (a,i,o) */
    13,  /* dalawang libo        (a,i,o) */
    13,  /* tatlong libo         (a,i,o) */
    13,  /* apat na libo         (a,i,o) */
    13,  /* limang libo          (a,i,o) */
    13,  /* anim na libo         (a,i,o) */
    13,  /* pitong libo          (a,i,o) */
    13,  /* walong libo          (a,i,o) */
    13,  /* siyam na libo        (a,i,o) */
    29   /* sampu libo (10k)     (a,i,o,u) */
};

static const unsigned LIBO_MASK = THOUSAND_MASK[1];   /* a,i,o */

/* Returns a bit mask of vowels present in the Tagalog name of n.  The
   mapping is: bit 0 = a, bit 2 = i, bit 3 = o, bit 4 = u. */
static unsigned vowel_mask(idx_t n) {
    unsigned mask = 0;

    if (n < 20) {
        if (n == 0) {
            mask = UNIT_MASK[0];
        } else if (n < 10) {
            mask = UNIT_MASK[n];
        } else if (n == 10) {
            mask = SAMPU_MASK;
        } else {
            mask = LABING_MASK | UNIT_MASK[n - 10];
        }
    } else if (n < 100) {
        idx_t t = n / 10u;
        mask = TENS_MASK[t - 2];
        idx_t u = n % 10u;
        if (u > 0u) mask |= UNIT_MASK[u];
    } else if (n < 1000) {
        idx_t h = n / 100u;
        mask = HUNDRED_MASK[h];
        n = n % 100u;
        if (n >= 20) {
            idx_t t = n / 10u;
            mask |= TENS_MASK[t - 2];
            n = n % 10u;
        }
        if (n > 0u) mask |= UNIT_MASK[n];
    } else {
        idx_t th = n / 1000u;
        idx_t rem = n % 1000u;
        mask = vowel_mask(th) | LIBO_MASK;
        if (rem != 0u) {
            mask |= vowel_mask(rem);
        }
    }

    return mask;
}

seq_t a(idx_t n) {
    unsigned mask;

    if (n == 0u) {
        mask = UNIT_MASK[0];
    } else {
        mask = vowel_mask(n);
    }

    unsigned count = 0;
    unsigned m = mask;
    while (m) {
        count++;
        m &= m - 1;
    }

    return (seq_t)count;
}
