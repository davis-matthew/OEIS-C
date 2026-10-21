/*
 * A093703  Numbers whose Roman numeral representation, reversed, is a Roman
 *          numeral.
 * https://oeis.org/A093703
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 30, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: pre‑compute all numbers <4000 whose Roman numeral
   representation, when reversed, is also a Roman numeral.  The
   function a(idx_t n) returns the n‑th such number (1‑based), using a
   static cache.  For n > sequence length the function returns 0.   */
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

static const unsigned ROMAN_VALUES[] = {
    1000, 900, 500, 400, 100, 90,
     50,  40,  10,   9,   5,   4,   1
};
static const char *ROMAN_SYMBOLS[] = {
    "M", "CM", "D", "CD", "C", "XC",
    "L", "XL", "X", "IX", "V", "IV", "I"
};
#define ROMAN_PAIRS (sizeof(ROMAN_VALUES)/sizeof(ROMAN_VALUES[0]))

/* Convert an integer to its canonical Roman numeral string.
   The buffer must be large enough (at least 20 bytes).           */
static void int_to_roman(unsigned int n, char *buf)
{
    size_t pos = 0;
    for (size_t i = 0; i < ROMAN_PAIRS && n > 0; ++i)
    {
        unsigned int val = ROMAN_VALUES[i];
        const char *sym = ROMAN_SYMBOLS[i];
        while (n >= val)
        {
            /* Append symbol to buf */
            for (const char *p = sym; *p; ++p)
                buf[pos++] = *p;
            n -= val;
        }
    }
    buf[pos] = '\0';
}

/* Parse a canonical Roman numeral string.
   Returns 1 if valid, 0 otherwise.  If valid, *val is set
   to the integer value.                                        */
static int parse_roman(const char *s, unsigned int *val)
{
    const char *p = s;
    unsigned int total = 0;
    for (size_t i = 0; i < ROMAN_PAIRS; ++i)
    {
        const char *sym = ROMAN_SYMBOLS[i];
        size_t len = strlen(sym);
        while (strncmp(p, sym, len) == 0)
        {
            total += ROMAN_VALUES[i];
            p += len;
        }
    }
    return (*p == '\0') ? (*val = total, 1) : 0;
}

/* Return the n-th member of the sequence (1‑based). */
seq_t a(idx_t n)
{
    static seq_t *seq = NULL;
    static size_t seq_len = 0;
    static int initialized = 0;

    if (!initialized)
    {
        /* Pre‑compute all valid numbers up to 3999 */
        const unsigned MAX_VAL = 3999;
        seq = (seq_t *)malloc(MAX_VAL * sizeof(seq_t));
        if (!seq) return (seq_t)0; /* allocation failure */

        char buf[20], rev[20];
        size_t idx = 0;
        unsigned int rev_val;

        for (unsigned int x = 1; x <= MAX_VAL; ++x)
        {
            int_to_roman(x, buf);

            /* Reverse buf into rev */
            size_t len = strlen(buf);
            for (size_t i = 0; i < len; ++i)
                rev[i] = buf[len - 1 - i];
            rev[len] = '\0';

            if (parse_roman(rev, &rev_val))
                seq[idx++] = (seq_t)x;
        }
        seq_len = idx;
        initialized = 1;
    }

    if (n == 0 || (idx_t)n > seq_len)
        return (seq_t)0;
    return seq[(size_t)n - 1];
}
