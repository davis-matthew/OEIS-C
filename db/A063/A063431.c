/*
 * A063431  Square array read by antidiagonals of n written in base k (n,k>0).
 * https://oeis.org/A063431
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, tabl
 * Formula:   a(n) is the base‑k representation of the row number r * written
 *            in base k, where r and k are obtained by reading the * infinite
 *            square array (n,k>0) along its antidiagonals. * For an index
 *            n≥1: * let t = ceil((√(8n+1)-1)/2) // minimal t with T_t ≥ n,
 *            T_t = t(t+1)/2 * let r = n – T_{t-1} // row nu...
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   decimal_len, pow10_di
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>
#include "../utils/decimal_len.h"
#include "../utils/pow10_di.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;              /* offset is 1; values before that are not part of the sequence */

    uint64_t N = n;

    /* find smallest t with T_t = t(t+1)/2 ≥ n */
    uint64_t t = (uint64_t)ceill((sqrtl((long double)8 * N + 1) - 1) / 2.0L);

    /* previous triangular number T_{t-1} = (t-1)*t/2 */
    uint64_t prev_tri = t * (t - 1) / 2;

    uint64_t offset = N - prev_tri;      /* row number r (1‑based) */
    uint64_t r = offset;
    uint64_t base = (t + 1) - offset;    /* column number / base, 1‑based */

    seq_t result = 0;

    /* special case: base 1 => r copies of digit 1 */
    if (base == 1) {
        for (uint64_t i = 0; i < r; ++i)
            result = result * 10 + 1;
        return result;
    }

    /* build digits of r in base 'base' (least‑to‑most) */
    if (r == 0) {
        return 0;
    }

    uint64_t digits[64];
    int dcnt = 0;
    uint64_t tmp = r;
    while (tmp > 0) {
        digits[dcnt++] = tmp % base;
        tmp /= base;
    }

    /* append digits from most‑to‑least, concatenating decimal representations */
    for (int i = dcnt - 1; i >= 0; --i) {
        uint64_t digit = digits[i];
        int len = decimal_len(digit);
        seq_t pow10 = pow10_di(len);
        result = result * pow10 + digit;
    }

    return result;
}
