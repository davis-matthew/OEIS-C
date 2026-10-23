/*
 * A123060  Least positive integer k such that n has the same number of
 *          characters in base k and in Roman numeral representation, or 0 if
 *          no such k exists.
 * https://oeis.org/A123060
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = least k >= 1 such that len_k(n) = len_ROMAN(n), or 0 if
 *            no such k exists. Here len_k(n) is the number of digits of n
 *            written in base k, and len_ROMAN(n) is the number of characters
 *            of the Roman numeral for n
 * Range:     u8 n <= 499, u16 n <= 3999, u32 n <= 3999, u64 n <= 3999,
 *            u128 n <= 3999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static int roman_len(idx_t n) {
    int len = 0;
    /* values and corresponding symbol lengths for standard Roman numerals */
    static const idx_t values[]  = {1000, 900, 500, 400, 100, 90,  50, 40, 10, 9, 5, 4, 1};
    static const int   s_len[]  = {1,    2,  1,    2,   1,  2,   1, 2,  1, 2, 1, 2, 1};
    for (int i = 0; i < 13; ++i) {
        while (n >= values[i]) {
            n -= values[i];
            len += s_len[i];
        }
    }
    return len;
}

/* seq_t a(idx_t n) */
seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;            /* outside offset */

    int rlen = roman_len(n);               /* length of Roman(n) */

    /* base 1 works iff the number of digits in unary equals Roman length */
    if (n == (idx_t)rlen) return (seq_t)1;

    /* Roman length 1 means base must be > n (since only base > n gives a single digit) */
    if (rlen == 1) return (seq_t)(n + 1);

    /* search for the minimal k >= 2 */
    for (idx_t k = 2; k <= n; ++k) {
        idx_t tmp = n;
        int   blen = 0;
        do {
            tmp /= k;
            ++blen;
        } while (tmp > 0);

        if (blen == rlen) return (seq_t)k;
        if (blen <  rlen) break;          /* further k will only shorten the representation */
    }

    return (seq_t)0;                       /* no suitable base found */
}
