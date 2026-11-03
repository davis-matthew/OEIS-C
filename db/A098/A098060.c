/*
 * A098060  Numbers whose French names end in a vowel.
 * https://oeis.org/A098060
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  word, nonn
 * Formula:   a(n) = the nth non‑negative integer whose French name ends in a
 *            vowel. Computed by searching numbers incrementally and testing
 *            the last letter of the French representation
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Validated: partial, pass, 59 terms through n = 59 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* last letter of numbers 1–19 (index 0 unused) */
static inline char last_letter_under_20(unsigned int n)
{
    static const char map[20] = {
        0,        /* 0: not used */
        'n',      /* 1: un     */
        'x',      /* 2: deux   */
        's',      /* 3: trois  */
        'e',      /* 4: quatre */
        'q',      /* 5: cinq   */
        'x',      /* 6: six    */
        't',      /* 7: sept   */
        't',      /* 8: huit   */
        'f',      /* 9: neuf   */
        'x',      /*10: dix    */
        'e',      /*11: onze   */
        'e',      /*12: douze  */
        'e',      /*13: treize */
        'e',      /*14: quatorze*/
        'e',      /*15: quinze */
        'e',      /*16: seize  */
        't',      /*17: dix‑sept*/
        't',      /*18: dix‑huit*/
        'f'       /*19: dix‑neuf*/
    };
    return map[n];
}

/* last letter of the French name of n */
static inline char last_french_letter(uint64_t n)
{
    if (n == 0)            /* "zero" -> 'o' */
        return 'o';

    if (n < 20)            /* short numbers */
        return last_letter_under_20((unsigned int)n);

    if (n < 100) {
        unsigned int units = (unsigned int)(n % 10);
        unsigned int tens  = (unsigned int)(n / 10);

        if (tens == 1)     /* 10–19 already handled by <20 */
            return last_letter_under_20((unsigned int)n);

        if (tens == 7)                /* 70–79: 60 + (n−60) */
            return last_french_letter(n - 60);

        if (tens == 8) {
            if (units == 0)           /* 80: "quatre‑vingts" */
                return 's';
            return last_french_letter(units);
        }

        if (tens == 9)                /* 90–99: 80 + (n−80) */
            return last_french_letter(n - 80);

        /* tens 2–6 : standard pattern */
        if (units == 0) {
            switch (tens) {
                case 2: return 't'; /* vingt */
                case 3: return 'e'; /* trente */
                case 4: return 'e'; /* quarante */
                case 5: return 'e'; /* cinquante */
                case 6: return 'e'; /* soixante */
                default: return 'e';
            }
        }
        return last_french_letter(units);
    }

    if (n < 1000) {
        unsigned int hundreds = (unsigned int)(n / 100);
        unsigned int rem      = (unsigned int)(n % 100);

        if (rem == 0) {               /* exact hundred */
            if (hundreds == 1)
                return 't';           /* cent */
            return 's';              /* multiples of 100 >100 */
        }
        /* otherwise last word = remainder */
        return last_french_letter(rem);
    }

    /* thousands and above */
    unsigned int remainder = (unsigned int)(n % 1000);
    if (remainder == 0)               /* multiple of 1000 -> "mille" */
        return 'e';
    return last_french_letter(remainder);
}

/* true if n’s French name ends with a vowel letter */
static inline bool ends_with_vowel(uint64_t n)
{
    char c = last_french_letter(n);
    return c=='a' || c=='e' || c=='i' || c=='o' || c=='u' || c=='y';
}

/* ---------- sequence storage and construction ---------- */

static seq_t *seq_cache = NULL;
static size_t seq_len   = 0;
static size_t seq_cap   = 0;
static uint64_t next_check = 0;   /* next integer to test */

/* grow the cache */
static void ensure_capacity(size_t needed)
{
    if (seq_cap >= needed)
        return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 64;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_cache = mem_resize(seq_t, seq_cache, new_cap);
    seq_cap   = new_cap;
}

/* ---------- required function ---------- */

seq_t a(idx_t n)
{
    if (n == 0)               /* offset is 1, but guard against 0 */
        return 0;

    while (seq_len < (size_t)n) {
        /* find the next integer with a vowel ending */
        while (!ends_with_vowel(next_check))
            next_check++;

        ensure_capacity(seq_len + 1);
        seq_cache[seq_len++] = (seq_t)next_check;
        next_check++;          /* move to the next candidate */
    }
    return seq_cache[n - 1];
}
