/*
 * A037878  (1/2)*Sum{|d(i)-e(i)|}, where Sum{d(i)*10^i} is base 10
 *          representation of n and e(i) are digits d(i) in nonincreasing
 *          order, for i=0,1,...,m.
 * https://oeis.org/A037878
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (1/2) * Σ |d_i - e_i|, where d_i are the decimal digits
 *            of n and e_i are those digits sorted in non‑increasing order
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <string.h>

seq_t a(idx_t n) {
    /* For n < 1 the behaviour is unspecified; return 0. */
    if (n == 0) {
        return (seq_t)0;
    }

    /* Extract decimal digits of n.  d[0] is the units digit.  For a 64‑bit
       integer the number of decimal digits is at most 20. */
    unsigned char d[20];
    size_t len = 0;
    idx_t tmp = n;
    while (tmp > 0) {
        d[len++] = (unsigned char)(tmp % 10);
        tmp /= 10;
    }
    if (len == 0) {
        d[len++] = 0;
    }

    /* Sort the digits in non‑increasing order to obtain e[i]. */
    unsigned char e[20];
    memcpy(e, d, len);
    for (size_t i = 0; i < len; ++i) {
        for (size_t j = i + 1; j < len; ++j) {
            if (e[j] > e[i]) {
                unsigned char t = e[i];
                e[i] = e[j];
                e[j] = t;
            }
        }
    }

    /* Compute the sum of absolute differences and halve it. */
    seq_t sum = 0;
    for (size_t i = 0; i < len; ++i) {
        int diff = e[i] > d[i] ? e[i] - d[i] : d[i] - e[i];
        sum += (seq_t)diff;
    }
    return sum / 2;
}
