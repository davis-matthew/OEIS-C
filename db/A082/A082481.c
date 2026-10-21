/*
 * A082481  Number of 1's in binary representation of C(2n,n).
 * https://oeis.org/A082481
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of 1's in the binary representation of C(2n,n)
 * Range:     u8 n <= 244, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   div_uint64, mul_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <assert.h>
#include "../utils/div_uint64.h"
#include "../utils/mul_uint64.h"

/*  Main sequence function  */
seq_t a(idx_t n)
{
    /*  Big integer stored as an array of 64‑bit limbs (little endian).
        For n <= 10000 the number of limbs needed is < 400, so use 512.  */
    uint64_t limbs[512] = {0};
    size_t   len  = 1;
    limbs[0] = 1;                /* C(0,0) = 1 */

    for (idx_t k = 1; k <= n; ++k) {
        uint64_t mult = (uint64_t)n + (uint64_t)k;   /* multiply by n+k */
        uint64_t divi = (uint64_t)k;                 /* divide by k   */
        mul_uint64(limbs, &len, mult);
        div_uint64(limbs, &len, divi);
    }

    /* Count 1 bits in the resulting big integer.  */
    uint64_t pop = 0;
    for (size_t i = 0; i < len; ++i)
        pop += (uint64_t)__builtin_popcountll(limbs[i]);

    return (seq_t)pop;          /* truncates naturally if seq_t is smaller */
}
