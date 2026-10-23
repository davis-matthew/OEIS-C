/*
 * A163597  The smallest pronic (A002378) that contains the digits of n in its
 *          exact middle.
 * https://oeis.org/A163597
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the smallest pronic number m = k·(k+1) such that the
 *            decimal digits of n appear in the exact middle of m. That is, if
 *            m has L decimal digits and n has d digits, then L ≥ d, (L−d) is
 *            even, and (m / 10^((L−d)/2)) mod 10^d == n
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/pow10.h"
#include "../utils/digit_len.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;                 /* a(0) = 0 by definition */

    seq_t n_seq = (seq_t)n;
    size_t n_len = digit_len(n_seq);       /* number of decimal digits of n */
    seq_t pow10_n = pow10((idx_t)n_len);   /* 10^n_len, reused for every k */

    for (idx_t k = 0; ; ++k) {
        seq_t m = (seq_t)k * (k + 1);      /* pronic number */

        size_t m_len = digit_len(m);      /* decimal length of m */
        if (m_len < n_len) continue;      /* n cannot fit in the middle */

        if ( (m_len - n_len) & 1U ) continue;   /* (L-d) must be even */

        size_t s = (m_len - n_len) / 2;   /* digits to the right of the middle block */
        seq_t pow10_s = pow10((idx_t)s);  /* 10^s */

        /* Extract the middle block of length n_len */
        seq_t middle = (m / pow10_s) % pow10_n;

        if (middle == n_seq) return m;    /* found the smallest such pronic */
    }
}
