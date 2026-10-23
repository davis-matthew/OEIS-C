/*
 * A031006  a(n)=least k such that base 7 representation of n begins at s(k),
 *          where s=A030998.
 * https://oeis.org/A031006
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k such that the base‑7 representation of n begins
 *            at position k in s(k), where s is the concatenation of the
 *            base‑7 representations of the positive integers
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* ----- base‑7 representation of n as pattern ----- */
    char pat[32];
    size_t m = 0;
    idx_t t = n;
    do {
        pat[m++] = '0' + (char)(t % 7);
        t /= 7;
    } while (t);
    for (size_t i = 0; i < m/2; ++i) {
        char c = pat[i];
        pat[i] = pat[m-1-i];
        pat[m-1-i] = c;
    }

    /* ----- KMP prefix table for the pattern ----- */
    size_t pi[32];
    pi[0] = 0;
    for (size_t i = 1; i < m; ++i) {
        size_t j = pi[i-1];
        while (j > 0 && pat[i] != pat[j]) j = pi[j-1];
        if (pat[i] == pat[j]) ++j;
        pi[i] = j;
    }

    /* ----- Scan the infinite stream s ----- */
    size_t matched = 0;          /* length of current match */
    idx_t pos = 0;               /* current position in s (1‑based) */

    for (idx_t num = 1; ; ++num) {
        /* digits of num in base 7 (stored reversed) */
        char dbuf[32];
        size_t dlen = 0;
        idx_t u = num;
        do {
            dbuf[dlen++] = '0' + (char)(u % 7);
            u /= 7;
        } while (u);

        /* output digits in normal order (most‑significant first) */
        for (size_t i = 0; i < dlen; ++i) {
            char c = dbuf[dlen-1-i];
            ++pos;

            while (matched > 0 && pat[matched] != c)
                matched = pi[matched-1];

            if (pat[matched] == c)
                ++matched;

            if (matched == m)
                return (seq_t)(pos - m + 1);   /* earliest occurrence */
        }
    }
}
