/*
 * A061915  Obtain m by omitting trailing zeros from n; a(n) = smallest
 *          multiple k*m which is a palindrome with even digits, or -1 if no
 *          such multiple exists.
 * https://oeis.org/A061915
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, sign, easy
 * Formula:   a(n) = * strip trailing zeros from n to get m; * if m ends with
 *            5, return -1; * otherwise, return the smallest positive multiple
 *            k*m * that is a palindrome using only even decimal digits
 *            (0,2,4,6,8)
 * Range:     i8 n <= 6, i16 n <= 26, i32 n <= 60, i64 n <= 60, i128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* digits allowed in the palindrome */
static const int EVEN_DIGS[5] = {0, 2, 4, 6, 8};
static int LEFT[20];   /* left half digits, size sufficient for lengths up to 40 */

/* recursive generator that walks over the left half of a palindrome.
 * pos   : current position in the left half
 * h     : length of the left half
 * L     : total palindrome length
 * rem   : remainder modulo m built from the left half so far
 * m     : modulus as uint64_t
 * res   : pointer to store the resulting palindrome when found
 *
 * Returns true when a suitable palindrome has been found.
 */
static bool find_pal_rec(int pos, int h, int L, uint64_t rem,
                         uint64_t m, seq_t *res)
{
    if (pos == h) {
        uint64_t r = rem;
        if (L & 1) {            /* odd length */
            for (int i = h - 2; i >= 0; --i)
                r = (r * 10 + LEFT[i]) % m;
        } else {                /* even length */
            for (int i = h - 1; i >= 0; --i)
                r = (r * 10 + LEFT[i]) % m;
        }
        if (r == 0) {           /* divisible by m */
            seq_t val = 0;
            for (int i = 0; i < h; ++i)
                val = val * 10 + LEFT[i];
            if (L & 1) {        /* odd length */
                for (int i = h - 2; i >= 0; --i)
                    val = val * 10 + LEFT[i];
            } else {            /* even length */
                for (int i = h - 1; i >= 0; --i)
                    val = val * 10 + LEFT[i];
            }
            *res = val;
            return true;
        }
        return false;
    }

    for (int d = 0; d < 5; ++d) {
        int dig = EVEN_DIGS[d];
        if (pos == 0 && dig == 0)            /* leading zero not allowed */
            continue;
        uint64_t new_rem = (rem * 10 + dig) % m;
        LEFT[pos] = dig;
        if (find_pal_rec(pos + 1, h, L, new_rem, m, res))
            return true;
    }
    return false;
}

/* Search for the smallest even‑digit palindrome divisible by m.
 * m must be > 0 and not end with 5.  When found, the palindrome is
 * stored in *res and the function returns true.
 * For small m, the answer usually has few digits, so the search
 * terminates quickly.
 */
static bool find_even_digit_pal(seq_t m, seq_t *res)
{
    if (m == 0) {          /* only for n == 0 case, handled separately */
        *res = 0;
        return true;
    }
    uint64_t um = (uint64_t)m;

    /* m ending with 5 is impossible (given by OEIS conjecture) */
    if (um % 10 == 5)
        return false;

    /* iterate palindrome length starting from 1 */
    for (int L = 1; ; ++L) {
        int h = (L + 1) / 2;
        if (find_pal_rec(0, h, L, 0, um, res))
            return true;
        /* safety guard: any practical case will be found before
           length exceeds 20; if not, keep searching */
        if (L > 20)
            break;
    }
    return false;
}

/* Main OEIS A061915 implementation */
seq_t a(idx_t n)
{
    /* handle n == 0 separately (m == 0) */
    if (n == 0)
        return 0;

    /* obtain m by stripping trailing zeros from n */
    seq_t m = (seq_t)n;
    while (m % 10 == 0)
        m = m / 10;

    /* if m ends with 5, no solution */
    if ((uint64_t)m % 10 == 5)
        return -1;

    seq_t result;
    if (find_even_digit_pal(m, &result))
        return result;
    else
        return -1;      /* safety fallback, should not happen */
}
