/*
 * A167831  Largest m<=n such that no carry occurs when adding m to n in
 *          decimal arithmetic.
 * https://oeis.org/A167831
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn, look
 * Formula:   a(n) = largest m ≤ n such that adding m to n in decimal causes
 *            no carry. Algorithm: traverse the digits of n from
 *            most‑significant to least‑significant. For each digit position,
 *            the maximum allowed digit for m is 9 - n_digit. If all
 *            previously chosen digits of m equal the corresponding digits
 *            of...
 * Range:     u8 n <= 299, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n) {
    if (n == (idx_t)0) {
        return (seq_t)0;
    }

    /* Store the decimal digits of n, least‑significant first. */
    unsigned char digits[20]; /* 20 digits is enough for a 64‑bit integer */
    size_t len = 0;
    idx_t tmp = n;
    while (tmp > 0) {
        digits[len++] = (unsigned char)(tmp % 10);
        tmp /= 10;
    }

    /* Build m from most‑significant to least‑significant digit. */
    seq_t m = (seq_t)0;
    bool equal = true;               /* true while prefix of m equals prefix of n */
    for (size_t idx = len; idx > 0; --idx) {
        unsigned int n_digit = (unsigned int)digits[idx - 1];
        unsigned int max_digit = 9u - n_digit;
        unsigned int m_digit;

        if (equal) {
            if (n_digit <= max_digit) {
                m_digit = n_digit;   /* keep equality */
            } else {
                m_digit = max_digit; /* must go below n_digit */
                equal = false;
            }
        } else {
            m_digit = max_digit;     /* already smaller than n, use maximum allowed */
        }

        m = m * (seq_t)10 + (seq_t)m_digit;
    }

    return m;
}
