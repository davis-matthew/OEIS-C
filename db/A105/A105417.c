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
 * Helpers:   int_to_roman
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: search 1..3888 for numbers whose Roman numeral
   contains each of I, V, X, L, C, D, M at least once. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/int_to_roman.h"

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
