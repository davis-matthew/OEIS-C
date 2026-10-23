/*
 * A038557  Periodic derivative of n in base 3.
 * https://oeis.org/A038557
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = integer whose base‑3 representation has digits
 *            (b_k+b_{k-1}) (b_{k-1}+b_{k-2}) … (b_1+b_0) (b_0+b_k), where n =
 *            b_k … b_0 in base‑3 and additions are taken modulo 3
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    uint8_t d[64];      /* base‑3 digits of n, least significant first */
    size_t len = 0;
    uint64_t t = n;
    do
    {
        d[len++] = (uint8_t)(t % 3);
        t /= 3;
    } while (t != 0);

    seq_t res = 0;
    seq_t mul = 1;      /* current 3^i */

    for (size_t i = 0; i < len; ++i)
    {
        uint8_t sum = d[i] + d[(i + len - 1) % len]; /* b_i + b_{i-1} mod 3 */
        if (sum >= 3) sum -= 3;
        res += (seq_t)sum * mul;
        mul *= 3;
    }
    return res;
}
