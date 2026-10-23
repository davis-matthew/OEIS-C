/*
 * A181352  In lunar arithmetic, n*(n+1).
 * https://oeis.org/A181352
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n * (n + 1) in lunar arithmetic where the addition is
 *            lunar addition (digit‑wise max)
 * Range:     u8 n <= 29, u16 n <= 653, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Convert the index to the arithmetic type */
    seq_t n_val = (seq_t)n;

    /* Arrays to hold decimal digit strings (least significant first) */
    uint8_t dn[64] = {0};
    uint8_t dm[64] = {0};
    uint8_t res[128] = {0};

    size_t len_n = 0;   /* number of digits of n */

    /* Extract digits of n */
    seq_t tmp = n_val;
    if (tmp == (seq_t)0) {
        dn[0] = 0;
        len_n = 1;
    } else {
        while (tmp != (seq_t)0) {
            dn[len_n++] = (uint8_t)(tmp % (seq_t)10);
            tmp /= (seq_t)10;
        }
    }

    /* Construct digits of (n + 1) in lunar arithmetic:
       For all positions except the least‑significant one, the digit
       stays the same.  In the units place we take the maximum of
       that digit and 1.                                     */
    for (size_t i = 0; i < len_n; ++i) {
        dm[i] = dn[i];
    }
    if (dm[0] < 1) dm[0] = 1;
    size_t len_m = len_n;   /* same length as n */

    /* Compute the lunar product via convolution and digitwise max */
    size_t len_res = len_n + len_m;   /* room for all partial products */
    for (size_t i = 0; i < len_n; ++i) {
        for (size_t j = 0; j < len_m; ++j) {
            uint8_t val = dn[i] < dm[j] ? dn[i] : dm[j];  /* min(dn[i],dm[j]) */
            if (val > res[i + j]) {
                res[i + j] = val;
            }
        }
    }

    /* Trim leading zeroes */
    int hi = (int)len_res - 1;
    while (hi > 0 && res[hi] == 0) {
        --hi;
    }

    /* Convert the digit array back to a seq_t value */
    seq_t ans = 0;
    for (int k = hi; k >= 0; --k) {
        ans = ans * (seq_t)10 + res[k];
    }

    return ans;
}
