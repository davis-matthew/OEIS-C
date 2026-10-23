/*
 * A163499  Take A163498(n) written in binary, insert a 0 before every 1. a(n)
 *          is the decimal equivalent of the result.
 * https://oeis.org/A163499
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the nth number obtained by taking a prime p from sequence
 *            A163498 and inserting a 0 before every 1 in the binary
 *            representation of p. The result is returned as a seq_t
 * Range:     u8 n <= 6, u16 n <= 39, u32 n <= 44, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   bitlen_seq_t, is_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/bitlen_seq_t.h"
#include "../utils/is_prime.h"
#include "../utils/is_prime_big.h"

/* Insert a 0 before every 1 in the binary representation of p.
   For example, if p = 5 (binary 101), the function returns
   1001 (decimal 9). */
static inline seq_t transform(seq_t p)
{
    seq_t res = 0;
    int bits = (int)bitlen_seq_t(p);
    for (int i = bits - 1; i >= 0; --i) {
        res <<= 1;                            /* output 0 */
        if (((p >> i) & 1) != 0) {            /* original bit 1 -> output 1 */
            res <<= 1;
            res |= 1;
        }
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) abort(); /* OEIS offset starts at 1 */

    idx_t count = 0;
    idx_t candidate = 2;

    while (1) {
        if (is_prime(candidate)) {
            seq_t val = transform((seq_t)candidate);
            if (is_prime_big(val)) {
                ++count;
                if (count == n) return val;
            }
        }
        candidate += (candidate == 2 ? 1 : 2); /* iterate over odd numbers only */
    }
}
