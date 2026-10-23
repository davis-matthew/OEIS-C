/*
 * A159547  Smallest number b such that the number whose digits are n in base
 *          b is a skinny number.
 * https://oeis.org/A159547
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest base b >= maxdigit(n)+1 such that sumdigits_b((Σ
 *            d_i b^i)^2) = (Σ d_i)^2, where d_i are decimal digits of n
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

#define MAX_DIGITS 64   /* enough for 64‑bit integers */

/* Compute the OEIS sequence A159547 */
seq_t a(idx_t n)
{
    /* Extract decimal digits of n (least significant first) */
    idx_t t = n;
    int digits[MAX_DIGITS];
    int len = 0;
    int maxdig = 0;
    while (t > 0) {
        int d = t % 10;
        digits[len++] = d;
        if (d > maxdig) maxdig = d;
        t /= 10;
    }
    if (len == 0) { digits[len++] = 0; maxdig = 0; }

    /* Sum of decimal digits */
    seq_t s = 0;
    for (int i = 0; i < len; ++i) s += digits[i];

    /* Convolution coefficients for X^2 */
    int coeff_len = 2 * len - 1;
    seq_t coeff[MAX_DIGITS * 2] = {0};
    for (int i = 0; i < len; ++i)
        for (int j = 0; j < len; ++j)
            coeff[i + j] += (seq_t)digits[i] * (seq_t)digits[j];

    /* Find maximum coefficient to bound the search */
    seq_t maxC = 0;
    for (int i = 0; i < coeff_len; ++i)
        if (coeff[i] > maxC) maxC = coeff[i];

    /* Base search bounds */
    seq_t startB = (seq_t)maxdig + 1;
    if (startB < 2) startB = 2;          /* base 1 is invalid */
    seq_t upperB = maxC + 1;

    /* Temporary array for carry simulation (one extra slot) */
    seq_t tmp[MAX_DIGITS * 2] = {0};

    /* Relative target for comparison (use 64‑bit to avoid overflow) */
    uint64_t target = (uint64_t)s * (uint64_t)s;

    for (seq_t b = startB; b <= upperB; ++b) {
        /* Copy coefficients into tmp */
        for (int i = 0; i < coeff_len; ++i) tmp[i] = coeff[i];
        tmp[coeff_len] = 0;           /* clear carry slot */

        seq_t sum = 0;
        for (int i = 0; i < coeff_len; ++i) {
            seq_t c = tmp[i] / b;
            seq_t d = tmp[i] % b;
            sum += d;
            if (c > 0) tmp[i + 1] += c;
        }
        /* Process leftover carries in the last slot */
        seq_t carry = tmp[coeff_len];
        while (carry > 0) {
            sum += carry % b;
            carry /= b;
        }

        if ((uint64_t)sum == target) return b;
    }

    /* Should never be reached: default fallback */
    return upperB;
}
