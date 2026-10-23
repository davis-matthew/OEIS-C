/*
 * A120390  Sum of digits of double factorial numbers.
 * https://oeis.org/A120390
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = sum of decimal digits of n!! (double factorial)
 * Range:     u8 n <= 80, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Big‐integer product of the double factorial stored in base 10.  */
seq_t a(idx_t n)
{
    /* n ≥ 1 for all relevant calls; return 0 for safety on n==0. */
    if (n == 0) return (seq_t)0;

    /* dynamic array of decimal digits (least significant first). */
    size_t cap   = 256;          /* initial capacity */
    uint8_t *dig = (uint8_t*)malloc(cap * sizeof(uint8_t));
    if (!dig) return (seq_t)0;   /* allocation failure → return 0 */

    size_t len = 1;
    dig[0] = 1;                 /* start product = 1 */

    /* iterate over all factors of the same parity as n.  */
    idx_t f_start = (n & 1) ? 1 : 2;   /* 1 for odd, 2 for even */
    for (idx_t f = f_start; f <= n; f += 2) {
        uint32_t carry = 0;
        for (size_t i = 0; i < len; ++i) {
            uint32_t t = (uint32_t)dig[i] * (uint32_t)f + carry;
            dig[i] = t % 10;
            carry  = t / 10;
        }
        while (carry) {
            if (len == cap) {                     /* expand buffer */
                cap <<= 1;
                uint8_t *tmp = (uint8_t*)realloc(dig, cap * sizeof(uint8_t));
                if (!tmp) { free(dig); return (seq_t)0; }
                dig = tmp;
            }
            dig[len++] = carry % 10;
            carry     /= 10;
        }
    }

    /* compute sum of digits */
    uint64_t sum = 0;
    for (size_t i = 0; i < len; ++i) sum += dig[i];

    free(dig);
    return (seq_t)sum;   /* cast is safe because the digit sum fits in 64 bits */
}
