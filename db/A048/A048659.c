/*
 * A048659  Positions of letters in English alphabet (U.S. pronunciation) that
 *          rhyme with 'e'.
 * https://oeis.org/A048659
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dumb, fini, full, nonn, word
 * Formula:   a(n) = the alphabet position of the nth letter whose English
 *            pronunciation ends with "ee" (rhymes with 'e')
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Validated: full, pass, 9 terms through n = 9 (2026-11-02)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

/* Check whether a phonetic name ends with the substring "ee" */
static bool rhymes_with_e(const char *name)
{
    size_t len = 0;
    while (name[len] != '\0') len++;
    return len >= 2 && name[len - 2] == 'e' && name[len - 1] == 'e';
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for non‑positive indices */

    /* Phonetic names of the 26 English alphabet letters (US pronunciation) */
    static const char * const names[26] = {
        "eh",      /* A */
        "bee",     /* B */
        "see",     /* C */
        "dee",     /* D */
        "ee",      /* E */
        "eff",     /* F */
        "jee",     /* G */
        "aitch",   /* H */
        "eye",     /* I */
        "jay",     /* J */
        "kay",     /* K */
        "ell",     /* L */
        "em",      /* M */
        "en",      /* N */
        "oh",      /* O */
        "pee",     /* P */
        "cue",     /* Q */
        "ar",      /* R */
        "ess",     /* S */
        "tee",     /* T */
        "you",     /* U */
        "vee",     /* V */
        "double u",/* W */
        "ex",      /* X */
        "why",     /* Y */
        "zee"      /* Z */
    };

    seq_t pos = 0;   /* value to return */
    idx_t count = 0;

    for (idx_t i = 0; i < 26; ++i) {
        if (rhymes_with_e(names[i])) {
            ++count;
            if (count == n) {
                pos = (seq_t)(i + 1);   /* alphabet positions are 1‑based */
                break;
            }
        }
    }

    /* If n exceeds the number of rhyming letters, return 0 (undefined value) */
    return pos;
}
