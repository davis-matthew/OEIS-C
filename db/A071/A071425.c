/*
 * A071425  Total number of 1's in binary representation of all factorials
 *          from 1 to n.
 * https://oeis.org/A071425
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = sum_{i=1..n} popcount(i!)
 * Range:     u8 n <= 21, u16 n <= 233, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    seq_t total = (seq_t)0;

    /* Big integer stored in little‑endian 32‑bit limbs */
    uint32_t *fact = NULL;
    size_t fact_len = 0;
    size_t cap = 1;

    fact = (uint32_t *)malloc(sizeof(uint32_t) * cap);
    if (!fact)
        return (seq_t)0;         /* out of memory – return 0 */
    fact[0] = 1;                 /* 0! = 1 */
    fact_len = 1;

    for (idx_t i = 1; i <= n; ++i) {
        /* multiply current factorial by i */
        uint64_t carry = 0;
        for (size_t j = 0; j < fact_len; ++j) {
            uint64_t prod = (uint64_t)fact[j] * (uint64_t)i + carry;
            fact[j] = (uint32_t)prod;
            carry   = prod >> 32;
        }
        while (carry) {
            if (fact_len == cap) {
                cap <<= 1;
                fact = (uint32_t *)realloc(fact, sizeof(uint32_t) * cap);
                if (!fact)
                    return total; /* out of memory – return partial sum */
            }
            fact[fact_len++] = (uint32_t)carry;
            carry >>= 32;
        }

        /* popcount of the new factorial */
        unsigned pop = 0;
        for (size_t j = 0; j < fact_len; ++j)
            pop += __builtin_popcount(fact[j]);

        total += (seq_t)pop;
    }

    free(fact);
    return total;
}
