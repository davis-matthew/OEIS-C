/*
 * A086044  Position of the first occurrence of n in concatenated numbers
 *          (A007376, Champernowne constant).
 * https://oeis.org/A086044
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 123, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: incremental KMP search on Champernowne constant stream */

#include <stdint.h>

seq_t a(idx_t n) {
    /* convert n to decimal string */
    char pat[25];
    int L = 0;
    if (n == 0) {
        pat[0] = '0';
        L = 1;
    } else {
        uint64_t tmp = n;
        char rev[25];
        int revlen = 0;
        while (tmp > 0) {
            rev[revlen++] = '0' + (tmp % 10);
            tmp /= 10;
        }
        for (int i = revlen - 1; i >= 0; --i)
            pat[L++] = rev[i];
    }

    /* build prefix function */
    int pi[25];
    pi[0] = 0;
    for (int i = 1; i < L; ++i) {
        int j = pi[i - 1];
        while (j > 0 && pat[i] != pat[j]) j = pi[j - 1];
        if (pat[i] == pat[j]) ++j;
        pi[i] = j;
    }

    /* incremental KMP over concatenated numbers */
    idx_t pos = 0;          /* current position (1‑based after each digit) */
    int k = 0;              /* matched prefix length */
    idx_t m = 0;            /* current number producing digits */
    for (;;) {
        /* produce digits of m */
        char digits[25];
        int dlen = 0;
        if (m == 0) {
            digits[dlen++] = '0';
        } else {
            uint64_t t = m;
            char rev[25];
            int revlen = 0;
            while (t > 0) {
                rev[revlen++] = '0' + (t % 10);
                t /= 10;
            }
            for (int i = revlen - 1; i >= 0; --i)
                digits[dlen++] = rev[i];
        }

        /* scan each digit */
        for (int idx = 0; idx < dlen; ++idx) {
            char c = digits[idx];
            ++pos;                             /* move to next position */
            while (k > 0 && pat[k] != c) k = pi[k - 1];
            if (pat[k] == c) ++k;
            if (k == L) {
                idx_t result = pos - L + 1;
                return (seq_t)result;
            }
        }
        ++m;
    }
}
