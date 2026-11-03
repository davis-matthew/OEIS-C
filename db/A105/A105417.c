/*
 * A105417  Numbers that are pandigital in Roman numerals, using each of the
 *          symbols I, V, X, L, C, D and M at least once.
 * https://oeis.org/A105417
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, fini, full, easy, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 192, u32 n <= 192, u64 n <= 192, u128 n <= 192,
 *            big any n
 * Validated: full, pass, 192 terms through n = 192 (2026-11-02)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: search 1..3888 for numbers whose Roman numeral
   contains each of I, V, X, L, C, D, M at least once. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Convert an integer (1..3888) to its standard Roman
   numeral representation. The result is stored in 'buf'
   and is null‑terminated. */
static void int_to_roman(idx_t n, char *buf)
{
    static const int vals[] =  {1000, 900, 500, 400, 100, 90, 50, 40,
                                10,   9,   5,   4,   1};
    static const char *syms[] = {"M","CM","D","CD","C","XC","L","XL",
                                 "X","IX","V","IV","I"};
    int i, pos = 0;
    for (i = 0; i < 13; ++i) {
        while (n >= vals[i]) {
            const char *s = syms[i];
            while (*s) {
                buf[pos++] = *s++;
            }
            n -= vals[i];
        }
    }
    buf[pos] = '\0';
}

/* Public entry point for the sequence. */
seq_t a(idx_t n)
{
    static bool initialized = false;
    static seq_t arr[200];
    static idx_t count = 0;

    if (!initialized) {
        initialized = true;
        for (idx_t i = 1; i <= 3888; ++i) {
            char buf[32];
            int_to_roman(i, buf);

            bool used[7] = {false, false, false, false, false, false, false};
            const char *p = buf;
            while (*p) {
                switch (*p) {
                case 'I': used[0] = true; break;
                case 'V': used[1] = true; break;
                case 'X': used[2] = true; break;
                case 'L': used[3] = true; break;
                case 'C': used[4] = true; break;
                case 'D': used[5] = true; break;
                case 'M': used[6] = true; break;
                }
                p++;
            }
            bool ok = true;
            for (int k = 0; k < 7; ++k) {
                if (!used[k]) { ok = false; break; }
            }
            if (ok) {
                arr[count++] = (seq_t)i;
            }
        }
    }

    if (n == 0 || n - 1 >= count) return (seq_t)0;
    return arr[n - 1];
}
