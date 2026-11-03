/*
 * A166874  Numbers with vertical symmetry when expressed as Roman numerals.
 * https://oeis.org/A166874
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   nth number whose Roman numeral uses only vertically symmetric *
 *            characters (I, V, X, M) and is a palindrome. For n > 8 the
 *            sequence * is 1000·(n − 8)
 * Range:     u8 n <= 8, u16 n <= 11, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Validated: partial, pass, 11 terms through n = 11 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

static bool is_sym_vert_unique(idx_t val)
{
    /* Standard Roman numeral map (values and strings) */
    struct { idx_t val; const char *sym; } const map[] = {
        {1000, "M"}, {900, "CM"}, {500, "D"}, {400, "CD"},
        {100,  "C"}, {90,  "XC"}, {50,  "L"}, {40,  "XL"},
        {10,   "X"}, {9,   "IX"}, {5,   "V"}, {4,   "IV"},
        {1,    "I"}
    };
    const size_t nmap = sizeof(map) / sizeof(map[0]);

    /* Buffer for the Roman representation (enough for values up to 3999) */
    char buf[32];
    size_t pos = 0;

    /* Build the Roman string, rejecting any symbol that contains a non‑allowed
     * character (L, C, D).  Allowed characters are I, V, X, M. */
    for (size_t i = 0; i < nmap; ++i) {
        while (val >= map[i].val) {
            const char *s = map[i].sym;
            while (*s) {
                if (*s != 'I' && *s != 'V' && *s != 'X' && *s != 'M')
                    return false;            /* forbidden letter found */
                if (pos + 1 >= sizeof(buf))
                    return false;            /* buffer overflow */
                buf[pos++] = *s++;
            }
            val -= map[i].val;
        }
    }
    if (pos == 0) return false;
    buf[pos] = '\0';

    /* Palindrome check */
    for (size_t i = 0; i < pos / 2; ++i) {
        if (buf[i] != buf[pos - 1 - i])
            return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* Values after the 8th term are simple multiples of 1000. */
    if (n > 8)
        return (seq_t)((n - 8) * 1000);

    /* For n ≤ 8, brute‑force search for the nth valid number. */
    idx_t count = 0;
    idx_t num   = 1;
    while (1) {
        if (is_sym_vert_unique(num)) {
            ++count;
            if (count == n)
                return (seq_t)num;
        }
        ++num;
    }
}
