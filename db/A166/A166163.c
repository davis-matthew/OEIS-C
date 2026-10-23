/*
 * A166163  a(n) = the largest possible prime made by inserting either a 0 or
 *          a 1 anywhere in the binary representation of the n-th prime, then
 *          converting to decimal.
 * https://oeis.org/A166163
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the largest prime obtainable by inserting a 0 or a 1 into
 *            the binary representation of the n‑th prime, then interpreting
 *            the result as a binary number
 * Range:     u8 n <= 31, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   get_prime, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/is_prime_small.h"

/* primitive prime test sufficient for the values produced ( < 1<<20 ) */
static inline bool is_prime(uint64_t v)
{
    return is_prime_small(v);
}

seq_t a(idx_t n)
{
    /* 1‑based nth prime */
    uint32_t p = get_prime(n);

    /* binary representation of p */
    int len = 0;
    uint32_t tmp = p;
    while (tmp) { len++; tmp >>= 1; }

    /* store bits msb → lsb */
    uint8_t bits[64];
    tmp = p;
    for (int i = 0; i < len; ++i)
        bits[i] = (tmp >> (len - 1 - i)) & 1U;

    uint64_t best = 0;

    /* try all insertion points (0 … len) and digits 0/1 */
    for (int pos = 0; pos <= len; ++pos) {
        for (int ins = 0; ins <= 1; ++ins) {
            uint64_t cand = 0;
            for (int i = 0; i < len + 1; ++i) {
                cand <<= 1;
                if (i == pos)                /* inserted bit */
                    cand |= (uint64_t)ins;
                else                         /* original bit */
                    cand |= (uint64_t)bits[(i < pos) ? i : i - 1];
            }
            if (cand > 1 && is_prime(cand) && cand > best)
                best = cand;
        }
    }

    /* sequence definition guarantees at least one prime, but keep
       fallback to 0 in the unlikely event none found */
    return (seq_t)best;
}
