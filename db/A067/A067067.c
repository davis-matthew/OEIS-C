/*
 * A067067  a(n) = product of nonzero digits of n! (A000142).
 * https://oeis.org/A067067
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = product of nonzero digits of n!
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 22, u64 n <= 35, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* trivial cases */
    if (n == 0 || n == 1)
        return (seq_t)1;

    /* base 1e9 big‑integer representation (little‑endian) */
    const uint32_t BASE = 1000000000u;
    uint32_t *words = (uint32_t *)malloc(16 * sizeof(uint32_t));
    size_t cap   = 16;
    size_t len   = 1;
    words[0] = 1;

    /* compute factorial as big integer */
    for (idx_t k = 2; k <= n; ++k) {
        uint64_t carry = 0;
        for (size_t i = 0; i < len; ++i) {
            uint64_t prod = (uint64_t)words[i] * (uint64_t)k + carry;
            words[i] = (uint32_t)(prod % BASE);
            carry   = prod / BASE;
        }
        while (carry) {
            if (len == cap) {
                cap *= 2;
                words = (uint32_t *)realloc(words, cap * sizeof(uint32_t));
            }
            words[len++] = (uint32_t)(carry % BASE);
            carry /= BASE;
        }
    }

    /* product of non‑zero decimal digits of the factorial */
    seq_t prod = 1;
    for (size_t idx = 0; idx < len; ++idx) {
        uint32_t block = words[idx];
        for (int d = 0; d < 9; ++d) {
            uint32_t digit = block % 10;
            if (digit != 0)
                prod *= (seq_t)digit;
            block /= 10;
        }
    }

    free(words);
    return prod;
}
