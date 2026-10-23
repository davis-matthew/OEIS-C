/*
 * A171558  Squares in lunar arithmetic in base 5 written in base 10.
 * https://oeis.org/A171558
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n ⊙ n (lunar multiplication in base 5)
 * Range:     u8 n <= 24, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Convert n to base‑5 digits (least significant first). */
    uint8_t d[64];           /* holds digits of n, 0–4 */
    size_t len = 0;
    idx_t t = n;
    while (t) {
        d[len++] = t % 5;
        t /= 5;
    }
    if (len == 0) {          /* n == 0 */
        len = 1;
        d[0] = 0;
    }

    /* Compute n ⊙ n in lunar arithmetic (base‑5). */
    size_t res_len = 2 * len - 1;
    uint8_t r[64];
    for (size_t p = 0; p < res_len; ++p) {
        uint8_t best = 0;
        for (size_t i = 0; i < len; ++i) {
            size_t j = p - i;
            if (j >= len) continue;
            uint8_t val = d[i] < d[j] ? d[i] : d[j];
            if (val > best) best = val;
        }
        r[p] = best;
    }

    /* Remove leading zeros. */
    while (res_len > 1 && r[res_len - 1] == 0)
        --res_len;

    /* Convert the base‑5 result to decimal. */
    seq_t result = 0;
    for (size_t k = res_len; k-- > 0; ) {
        result = result * (seq_t)5 + r[k];
    }
    return result;
}
