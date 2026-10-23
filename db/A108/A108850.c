/*
 * A108850  Number of 1's in the binary expansion of the repunits.
 * https://oeis.org/A108850
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = popcount((10^n-1)/9)
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Count the ones in the binary representation of the decimal repunit,
   i.e. (10^n-1)/9.  The algorithm builds the repunit incrementally
   using a little‑endian array of 64‑bit limbs.  Each step multiplies
   the current value by 10 and adds 1.  Finally the popcount of each
   limb is summed.  This works for any seq_t that supports the + and
   += operators. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* dynamic array of 64‑bit limbs (little endian) */
    size_t size = 1;          /* number of used limbs */
    size_t cap  = 1;          /* allocated capacity */
    uint64_t *limbs = (uint64_t *)malloc(cap * sizeof(uint64_t));
    limbs[0] = 0;

    for (idx_t i = 0; i < n; ++i) {
        /* multiply by 10 */
        __uint128_t carry = 0;
        for (size_t j = 0; j < size; ++j) {
            __uint128_t prod = (__uint128_t)limbs[j] * 10 + carry;
            limbs[j] = (uint64_t)prod;
            carry = prod >> 64;
        }
        if (carry) {
            if (size == cap) {
                cap *= 2;
                limbs = (uint64_t *)realloc(limbs, cap * sizeof(uint64_t));
            }
            limbs[size++] = (uint64_t)carry;
        }

        /* add 1 */
        carry = 1;
        size_t j = 0;
        while (carry) {
            if (j == size) {
                if (size == cap) {
                    cap *= 2;
                    limbs = (uint64_t *)realloc(limbs, cap * sizeof(uint64_t));
                }
                limbs[size++] = 0;
            }
            uint64_t sum = limbs[j] + carry;
            if (sum < limbs[j]) { /* overflow */
                limbs[j] = sum;
                carry = 1;
            } else {
                limbs[j] = sum;
                carry = 0;
            }
            ++j;
        }
    }

    /* popcount the limbs */
    seq_t result = 0;
    for (size_t j = 0; j < size; ++j)
        result += (seq_t)__builtin_popcountll(limbs[j]);

    free(limbs);
    return result;
}
