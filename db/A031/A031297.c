/*
 * A031297  a(n) is the least k such that the base-10 representation of n
 *          begins at s(k), where s=A007376.
 * https://oeis.org/A031297
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, look, base
 * Formula:   not stated
 * Range:     u8 n <= 123, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A031297
 *
 *  a(n) = the least index k such that the decimal representation of n
 *         begins at position k in the infinite string
 *         s = 0123456789101112131415...
 *  (indices are zero based; a(1)=1, a(12)=1, a(123)=1.)
 *
 *  Implementation: brute force KMP search on the concatenation of digits
 *  of the natural numbers.  Works for all n ≥ 1 and is fast enough for
 *  n up to 10 000 (or more).  The routine returns the index as a seq_t.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* pattern: decimal digits of n */
    unsigned char pat[25];
    int pat_len = 0;
    if (n == 0) {
        pat[0] = '0';
        pat_len = 1;
    } else {
        idx_t t = n;
        unsigned char tmp[25];
        int tlen = 0;
        while (t > 0) {
            tmp[tlen++] = '0' + (t % 10);
            t /= 10;
        }
        for (int i = tlen - 1; i >= 0; --i)
            pat[pat_len++] = tmp[i];
    }

    /* build longest‑prefix‑suffix array (lps) for KMP */
    int lps[25];
    lps[0] = 0;
    int len = 0;
    for (int i = 1; i < pat_len; ++i) {
        while (len > 0 && pat[i] != pat[len])
            len = lps[len - 1];
        if (pat[i] == pat[len])
            ++len;
        lps[i] = len;
    }

    /* search */
    idx_t pos = 0;      /* current position in the infinite string */
    int j = 0;          /* matched prefix length */

    for (idx_t num = 0; ; ++num) {
        /* digits of num, in forward order */
        unsigned char digits[25];
        int dlen = 0;
        if (num == 0) {
            digits[0] = '0';
            dlen = 1;
        } else {
            idx_t t = num;
            while (t > 0) {
                digits[dlen++] = '0' + (t % 10);
                t /= 10;
            }
            for (int i = 0; i < dlen / 2; ++i) {
                unsigned char c = digits[i];
                digits[i] = digits[dlen - 1 - i];
                digits[dlen - 1 - i] = c;
            }
        }

        for (int i = 0; i < dlen; ++i) {
            unsigned char ch = digits[i];
            while (j > 0 && ch != pat[j])
                j = lps[j - 1];
            if (ch == pat[j])
                ++j;
            if (j == pat_len) {
                /* pattern ends at position 'pos' */
                seq_t result = (seq_t)(pos - pat_len + 1);
                return result;
            }
            ++pos;
        }
    }
}
