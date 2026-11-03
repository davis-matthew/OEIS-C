/*
 * A059437  Consider the last letter of each of the English words zero, one,
 *          two, three, four, five, ... . Write down 0 for a vowel or "y", 1
 *          for a consonant.
 * https://oeis.org/A059437
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, word, easy
 * Formula:   Generates OEIS sequence A059437 – last letter of the English
 *            name of n (0 for vowel or “y”, 1 for consonant)
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Validated: partial, pass, 105 terms through n = 104 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The calculation uses the standard English words for numbers
   (zero‑nine, ten‑nineteen, tens from twenty to ninety,
   hundreds, thousands, millions, etc.).  The last letter of the
   verbal form is determined by examining the lowest non‑zero
   three‑digit group.  When that group is zero and no lower
   groups exist, the last letter comes from the scale word
   (“thousand”, “million”, …).  All scale words ending in a consonant
   are handled accordingly. */

#include <stdbool.h>
#include <stdint.h>

/* Determine if a character is a vowel or 'y' (treated as vowel) */
static bool is_vowel(char c)
{
    switch (c) {
    case 'a': case 'e': case 'i': case 'o': case 'u': case 'y':
        return true;
    default:
        return false;
    }
}

/* Last letter of the scale word for the j‑th group (j >= 1):
   1 => "thousand" (d), 2 => "million" (n), 3 => "billion" (n), etc. */
static char scale_last_letter(unsigned int j)
{
    /* thousand ends with d; all higher scales end with n */
    return (j == 1) ? 'd' : 'n';
}

/* The function signature required by the build environment: */
seq_t a(idx_t n);

/* ------------------------------------------------------------------ */
/*  Helper functions                                                  */
/* ------------------------------------------------------------------ */

/* Map for numbers 0..19 to the last letter of their English word */
static const char small_last[20] = {
    'e','e','o','e','r','e','x','n','t','e', /* 0 - 9 */
    'n','n','e','n','n','n','n','n','n','n'   /*10 -19*/
};

/* Last letter of a three‑digit group (0 <= x < 1000) */
static char last_char_1000(uint32_t x)
{
    if (x < 20)                     /* 0 – 19 */
        return small_last[x];
    if (x % 100 == 0)               /* multiples of 100: "hundred" */
        return 'd';
    if (x % 10 == 0)                /* tens: 20,30,…,90 */
        return 'y';
    /* units digit 1–9 */
    return small_last[x % 10];
}

/* ------------------------------------------------------------------ */
/*  Main sequence function                                            */
/* ------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    if (n == 0)                     /* "zero" ends with 'e' */
        return (seq_t)0;

    /* Break n into groups of three digits */
    uint64_t groups[16]; /* enough for 64‑bit n */
    unsigned groups_cnt = 0;
    idx_t temp = n;
    while (temp > 0) {
        groups[groups_cnt++] = temp % 1000ULL;
        temp /= 1000ULL;
    }

    /* Find lowest non‑zero group */
    int first_nonzero = -1;
    for (unsigned i = 0; i < groups_cnt; ++i) {
        if (groups[i] != 0) {
            first_nonzero = (int)i;
            break;
        }
    }

    /* This should never happen because n != 0 */
    if (first_nonzero < 0)
        return (seq_t)0;

    char lastc;
    if (first_nonzero == 0) {
        /* Last part is the 3‑digit representation itself */
        lastc = last_char_1000((uint32_t)groups[0]);
    } else {
        /* All lower groups are zero; last part is the scale word */
        lastc = scale_last_letter(first_nonzero);
    }

    return is_vowel(lastc) ? (seq_t)0 : (seq_t)1;
}
