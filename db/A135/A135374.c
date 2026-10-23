/*
 * A135374  Mersenne numbers with digits sorted in increasing order and zeros
 *          suppressed.
 * https://oeis.org/A135374
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = digits of 2^n‑1 sorted in increasing order with zeros
 *            suppressed (OEIS A135374)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* 1) Compute 2^n and subtract 1  */
    seq_t m = seq_pow((seq_t)2, n) - (seq_t)1;

    /* 2) Extract decimal digits of m (in reverse order)  */
    uint8_t digits[4000];
    size_t dlen = 0;

    seq_t temp = m;
    /* For n >= 1, m is never zero, but keep a guard for completeness */
    while (temp != (seq_t)0) {
        seq_t rem = temp % (seq_t)10;
        digits[dlen++] = (uint8_t)rem;
        temp = temp / (seq_t)10;
    }

    /* 3) Count digit frequencies  */
    unsigned int freq[10] = {0};
    for (size_t i = 0; i < dlen; ++i)
        ++freq[digits[i]];

    /* 4) Build result by appending digits 1..9 in ascending order   */
    seq_t result = (seq_t)0;
    for (int d = 1; d < 10; ++d) {
        unsigned int cnt = freq[d];
        for (unsigned int j = 0; j < cnt; ++j)
            result = result * (seq_t)10 + (seq_t)d;
    }

    return result;
}
