/*
 * A067850  Highest power of 2 not exceeding n!.
 * https://oeis.org/A067850
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(log2(n!))
 * Range:     u8 n <= 57, u16 n <= 5910, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Compute the exponent of the largest power of two <= n!.
   n is a non‑negative unsigned 64‑bit integer.  The result is
   returned as seq_t, the integer type used by the build system. */
seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* 0! = 1, 1! = 1 -> log₂(1) = 0 */

    size_t cap = 16;              /* initial limb array size */
    size_t sz  = 1;               /* number of limbs currently used */
    uint64_t *limbs = (uint64_t *)malloc(cap * sizeof(uint64_t));
    if (!limbs) return 0;         /* allocation failure → 0 */

    limbs[0] = 1;                 /* factorial starts at 1 */

    for (idx_t i = 2; i <= n; ++i) {
        unsigned __int128 carry = 0;
        for (size_t j = 0; j < sz; ++j) {
            unsigned __int128 prod = (unsigned __int128)limbs[j] *
                                     (unsigned long long)i + carry;
            limbs[j] = (uint64_t)prod;
            carry    = prod >> 64;
        }
        while (carry) {
            if (sz == cap) {
                cap <<= 1;
                uint64_t *tmp = (uint64_t *)realloc(limbs, cap * sizeof(uint64_t));
                if (!tmp) {
                    free(limbs);
                    return 0;
                }
                limbs = tmp;
            }
            limbs[sz++] = (uint64_t)carry;
            carry >>= 64;
        }
    }

    /* Determine highest set bit in the most significant limb. */
    size_t top   = sz - 1;
    uint64_t last = limbs[top];
    int msb = 63;
    while (msb >= 0 && !(last & ((uint64_t)1 << msb))) msb--;

    seq_t result = (seq_t)(top * 64 + msb);

    free(limbs);
    return result;
}
