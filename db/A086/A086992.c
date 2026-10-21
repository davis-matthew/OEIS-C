/*
 * A086992  Product of nonzero digits in n-th row of Pascal's triangle.
 * https://oeis.org/A086992
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = product of non‑zero digits of all binomial coefficients
 *            in the (n-1)‑th row of Pascal's triangle
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 11, u64 n <= 15, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* Compute a(idx_t n).  seq_t and idx_t are supplied by the build. */
seq_t a(idx_t n)
{
    if (n == 0)                /* defined for completeness */
        return (seq_t)0;

    /* Row index (0‑based).  */
    idx_t r = n - 1;

    /* Product of all non‑zero digits.  */
    seq_t product = 1;

    /* Big integer coefficients represented in base 1 000 000 000.  */
    /*  For r ≤ 9999 the number of blocks never exceeds 350.  */
    uint32_t coeff[400];
    size_t   coeff_len = 1;      /* number of used blocks */
    coeff[0] = 1;                /* C(r,0) = 1 */

    /* Pre‑computed powers of ten needed when extracting digits of a
       block that is not the most significant one.  */
    static const uint32_t pow10[10] = {
        1u, 10u, 100u, 1000u, 10000u, 100000u,
        1000000u, 10000000u, 100000000u, 1000000000u
    };

    for (idx_t k = 0; k <= r; ++k)
    {
        /* ---------- process current coefficient ---------- */

        /* Process most significant block (no leading zeros).  */
        size_t sz = coeff_len;
        if (sz > 0) {
            unsigned msb = coeff[sz - 1];
            /* Convert msb to decimal digits.  */
            char msbbuf[12];
            int i = 0;
            unsigned tmp = msb;
            if (tmp == 0) {
                msbbuf[i++] = '0';
            } else {
                while (tmp > 0) {
                    msbbuf[i++] = (char)('0' + tmp % 10);
                    tmp /= 10;
                }
                /* reverse */
                for (int j = 0; j < i / 2; ++j) {
                    char c = msbbuf[j];
                    msbbuf[j] = msbbuf[i - 1 - j];
                    msbbuf[i - 1 - j] = c;
                }
            }
            for (int j = 0; j < i; ++j) {
                uint32_t d = (uint32_t)(msbbuf[j] - '0');
                if (d) product *= (seq_t)d;
            }

            /* Process remaining blocks (9 digits each, with leading zeros).  */
            for (int block = (int)sz - 2; block >= 0; --block) {
                unsigned val = coeff[block];
                for (int pos = 8; pos >= 0; --pos) {
                    uint32_t d = (val / pow10[pos]) % 10;
                    if (d) product *= (seq_t)d;
                }
            }
        }

        /* ---------- update coefficient for next k ---------- */
        if (k < r) {
            /* Multiply by (r - k).  */
            uint32_t mul = (uint32_t)(r - k);
            uint64_t carry = 0;
            for (size_t i = 0; i < coeff_len; ++i) {
                uint64_t cur = (uint64_t)coeff[i] * mul + carry;
                coeff[i] = (uint32_t)(cur % 1000000000u);
                carry   = cur / 1000000000u;
            }
            if (carry) {
                coeff[coeff_len++] = (uint32_t)carry;
                /* carry may exceed base; normalize if necessary.  */
                while (carry >= 1000000000u) {
                    coeff[coeff_len++] = (uint32_t)(carry % 1000000000u);
                    carry /= 1000000000u;
                }
            }

            /* Divide by (k + 1) – the division is exact.  */
            uint32_t div = (uint32_t)(k + 1);
            uint64_t rem = 0;
            for (int i = (int)coeff_len - 1; i >= 0; --i) {
                uint64_t cur = coeff[i] + rem * 1000000000u;
                coeff[i] = (uint32_t)(cur / div);
                rem      = cur % div;
            }
            /* Trim leading zero block.  */
            while (coeff_len > 1 && coeff[coeff_len - 1] == 0)
                --coeff_len;
        }
    }

    return product;
}
