/*
 * A008536  Numbers whose American English name contains the letter 'n'.
 * https://oeis.org/A008536
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word
 * Formula:   not stated
 * Range:     u8 n <= 213, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the nth positive integer whose American English
   name contains the letter 'n'.  The sequence is defined by searching for
   such numbers; no closed‑form formula is known. */

#include <stdbool.h>

/* Flags indicating whether the English word for a number
   0–19 contains the letter 'n'. */
static const unsigned char below20_has_n[20] = {
    0, /* zero */
    1, /* one   */
    0, /* two   */
    0, /* three */
    0, /* four  */
    0, /* five  */
    0, /* six   */
    1, /* seven */
    0, /* eight */
    1, /* nine  */
    1, /* ten   */
    1, /* eleven */
    0, /* twelve */
    1, /* thirteen */
    1, /* fourteen */
    1, /* fifteen */
    1, /* sixteen */
    1, /* seventeen */
    1, /* eighteen */
    1  /* nineteen */
};

/* Flags indicating whether the tens word (20,30,…,90) contains 'n'. */
static const unsigned char tens_has_n[10] = {
    0, /* unused */
    0, /* unused */
    1, /* twenty   */
    0, /* thirty   */
    0, /* forty    */
    0, /* fifty    */
    0, /* sixty    */
    1, /* seventy  */
    0, /* eighty   */
    1  /* ninety   */
};

/* Return true iff the integer x has an American English name that
   contains the letter 'n'.  This function handles 0–999; all numbers
   ≥ 1000 automatically contain 'n' because of the word "thousand" */
static inline bool contains_n_uint64(idx_t x)
{
    if (x >= 1000u)
        return true;

    int h   = (int)(x / 100u);   /* hundreds digit (0–9) */
    int rem = (int)(x % 100u);   /* two‑digit remainder (0–99) */

    /* Any non‑zero hundreds part contributes "hundred" which contains
       'n', so all numbers 100–999 are valid. */
    if (h > 0)
        return true;

    /* Handle the remaining two digits. */
    if (rem >= 20) {
        int t = rem / 10;        /* tens digit (2–9) */
        int o = rem % 10;        /* ones digit (0–9) */
        if (tens_has_n[t])
            return true;         /* e.g., "twenty" or "seventy" or "ninety" */
        if (o > 0 && below20_has_n[o])
            return true;         /* e.g., "one", "seven", or "nine" */
    } else if (rem > 0) {
        if (below20_has_n[rem])
            return true;         /* e.g., "two", "ten", etc. */
    }

    return false;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t cand   = 1;

    while (1) {
        if (contains_n_uint64(cand)) {
            ++count;
            if (count == n)
                return (seq_t)cand;
        }
        ++cand;
    }
}
