/*
 * A081931  Diagonal of triangle in A081930.
 * https://oeis.org/A081931
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(1) = 0, a(2) = 22. For n ≥ 3, let m = n – 1 and let p be the
 *            palindrome obtained from m by writing m in decimal as d[0] …
 *            d[k-1] (most significant first) and then: – if d[k-1] is even, p
 *            = d[0] … d[k-1] d[k-2] … d[0]; – if d[k-1] is odd, p = d[0] …
 *            d[k-1] d[k-1] d[k-2] … d[0]. Then a(n) is the...
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)22;

    idx_t m = n - 1;
    /* extract digits of m in most‑significant‑first order */
    uint8_t digs[64];
    idx_t k = 0;
    idx_t tmp = m;
    do {
        digs[k++] = tmp % 10;
        tmp /= 10;
    } while (tmp);
    for (idx_t i = 0; i < k / 2; ++i) {
        uint8_t t = digs[i];
        digs[i] = digs[k - 1 - i];
        digs[k - 1 - i] = t;
    }

    uint8_t last = digs[k - 1];
    bool last_even = ((last & 1) == 0);
    idx_t len_p = last_even ? (2 * k - 1) : (2 * k);
    idx_t zeros = (n - 2 - len_p) / 2;

    seq_t result = (seq_t)1;
    /* leading zeros */
    for (idx_t i = 0; i < zeros; ++i) result = result * (seq_t)10;

    /* palindrome p */
    if (last_even) {
        /* digits of m */
        for (idx_t i = 0; i < k; ++i)
            result = result * (seq_t)10 + (seq_t)digs[i];
        /* reverse of all digits except the last one */
        for (idx_t i = 0; i < k - 1; ++i)
            result = result * (seq_t)10 + (seq_t)digs[k - 2 - i];
    } else {
        /* digits of m */
        for (idx_t i = 0; i < k; ++i)
            result = result * (seq_t)10 + (seq_t)digs[i];
        /* duplicate the last digit */
        result = result * (seq_t)10 + (seq_t)last;
        for (idx_t i = 0; i < k - 1; ++i)
            result = result * (seq_t)10 + (seq_t)digs[k - 2 - i];
    }

    /* trailing zeros */
    for (idx_t i = 0; i < zeros; ++i) result = result * (seq_t)10;
    /* final 1 */
    result = result * (seq_t)10 + (seq_t)1;

    return result;
}
