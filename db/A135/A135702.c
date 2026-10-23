/*
 * A135702  Decimal expansion of 3*Sum_{k=1..inf} 1/(10^k-1).
 * https://oeis.org/A135702
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = n-th digit after the decimal point of 3 *
 *            \sum_{k=1}^{\infty} 1/(10^k-1) (decimal expansion)
 * Range:     u8 n <= 2500, u16 n <= 2500, u32 n <= 2500, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

/*  external types seq_t and idx_t are supplied by the build system  */
extern seq_t a(idx_t n)
{
    /* A small buffer for propagating carries from lower positions. */
    const size_t SAFETY = 50;

    /* Need at least indices 0 .. n, plus safety room. */
    size_t len = (size_t)n + SAFETY + 1;
    uint32_t *raw = (uint32_t *)calloc(len, sizeof(uint32_t));
    if (!raw)
        return (seq_t)0;   /* unlikely, but safe fallback */

    /* For each k, add 3 at all positions where k divides (position+1). */
    for (size_t k = 1; k <= len; ++k) {
        for (size_t pos = k - 1; pos < len; pos += k) {
            raw[pos] += 3U;
        }
    }

    /* Propagate decimal carries from right to left. */
    for (size_t i = len; i > 0; --i) {
        size_t idx = i - 1;
        uint32_t carry = raw[idx] / 10U;
        raw[idx] %= 10U;
        if (idx > 0)
            raw[idx - 1] += carry;
        /* carry that would move left of index 0 is discarded
           – it changes only the integer part of the value. */
    }

    seq_t result = (seq_t)raw[n];
    free(raw);
    return result;
}
