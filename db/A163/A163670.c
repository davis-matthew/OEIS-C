/*
 * A163670  Numbers whose English name (excluding spaces and hyphens) is
 *          written with only straight line segments (chisel strokes).
 * https://oeis.org/A163670
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, fini, full, nonn, word
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   straight_word
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A163670: numbers whose English name uses only straight‑line letters
   (A, E, F, H, I, K, L, M, N, T, V, W, X, Y, Z). The function generates
   the sequence by testing each number 1..99 and returning the n‑th valid
   number. For indices beyond the length (16 elements) it returns 0. */
#include <stdbool.h>
#include "../utils/straight_word.h"

/* Static lookup tables for English words of numbers up to 99. */
static const char *const ones[20] = {
    /* 0 */ "", "one", "two", "three", "four", "five", "six", "seven",
    "eight", "nine", "ten", "eleven", "twelve", "thirteen", "fourteen",
    "fifteen", "sixteen", "seventeen", "eighteen", "nineteen"
};

static const char *const tens[10] = {
    /* 0 */ "", "", "twenty", "thirty", "forty", "fifty",
    "sixty", "seventy", "eighty", "ninety"
};

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;               /* undefined for non‑positive indices */
    idx_t count = 0;                    /* how many valid numbers found */
    for (int num = 1; num <= 99; ++num) {
        const char *w;
        if (num < 20) {
            w = ones[num];
        } else {
            int t = num / 10;
            int u = num % 10;
            /* Build the word on the fly in a static buffer to avoid allocations. */
            static char buf[32];
            /* copy tens part */
            const char *tstr = tens[t];
            int pos = 0;
            for (; *tstr && pos < (int)sizeof(buf) - 1; ++tstr, ++pos)
                buf[pos] = *tstr;
            if (u != 0) {
                const char *ustr = ones[u];
                for (; *ustr && pos < (int)sizeof(buf) - 1; ++ustr, ++pos)
                    buf[pos] = *ustr;
            }
            buf[pos] = '\0';
            w = buf;
        }
        if (straight_word(w)) {
            ++count;
            if (count == n) {
                return (seq_t)num;
            }
        }
    }
    /* If we run out of numbers (n > 16) return 0. */
    return 0;
}
