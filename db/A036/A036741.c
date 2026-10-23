/*
 * A036741  Values increase, Roman numerals increase lexicographically.
 * https://oeis.org/A036741
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 35, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence consists of the integers
   whose Roman numeral representation is lexicographically larger than
   that of the preceding accepted integer.
   To obtain the n‑th element we simulate the process:
   start with 1, then repeatedly try the next integer,
   accept it when its Roman numeral is > the previous one. */

#include <stdlib.h>
#include <string.h>

/* Lookup table for Roman notation (strongest pattern first). */
static const struct {
    unsigned long long val;
    const char *sym;
} roman_table[] = {
    {1000, "M"},
    {900,  "CM"},
    {500,  "D"},
    {400,  "CD"},
    {100,  "C"},
    {90,   "XC"},
    {50,   "L"},
    {40,   "XL"},
    {10,   "X"},
    {9,    "IX"},
    {5,    "V"},
    {4,    "IV"},
    {1,    "I"}
};

/* Convert a non‑negative 64‑bit integer to a Roman numeral string.  */
static char *to_roman(unsigned long long n, char *buf, size_t buflen)
{
    size_t pos = 0;
    for (size_t i = 0; i < sizeof(roman_table)/sizeof(roman_table[0]); ++i) {
        while (n >= roman_table[i].val) {
            const char *s = roman_table[i].sym;
            size_t len = strlen(s);
            if (pos + len >= buflen) {
                buf[pos] = '\0';
                return buf;
            }
            memcpy(buf + pos, s, len);
            pos += len;
            n -= roman_table[i].val;
        }
    }
    buf[pos] = '\0';
    return buf;
}

/* Sequence function (external linkage). */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static char prev[256] = "";               /* Roman of previous term */

    /* Ensure we have at least n terms cached. */
    size_t needed = (size_t)n;
    while (cache_len < needed) {
        /* Start from the smallest integer larger than the last accepted. */
        idx_t current = (cache_len == 0) ? 1 : (idx_t)cache[cache_len - 1] + 1;

        for (;;) {
            unsigned long long num = (unsigned long long)current;
            char cand[256];
            to_roman(num, cand, sizeof(cand));

            if (strcmp(cand, prev) > 0) {
                /* Accept current. Grow cache if necessary. */
                if (cache_len >= cache_cap) {
                    size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                    seq_t *newp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!newp) exit(1); /* allocation failure */
                    cache = newp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = (seq_t)current;

                /* Update the previous Roman string. */
                strncpy(prev, cand, sizeof(prev) - 1);
                prev[sizeof(prev) - 1] = '\0';

                break;  /* move on to next term */
            } else {
                /* Current integer does not produce a lexicographically larger
                   Roman numeral; try the next integer. */
                current++;
            }
        }
    }

    return cache[n - 1];
}
