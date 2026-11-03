/*
 * A082481  Number of 1's in binary representation of C(2n,n).
 * https://oeis.org/A082481
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of 1's in the binary representation of C(2n,n)
 * Range:     u8 n <= 244, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 89 terms through n = 9074 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <assert.h>

static inline void div_uint64(uint64_t *limbs, size_t *len, uint64_t d)
{
    unsigned __int128 rem = 0;
    for (int i = (int)*len - 1; i >= 0; --i) {
        unsigned __int128 cur = (rem << 64) | limbs[i];
        limbs[i] = (uint64_t)(cur / d);
        rem = cur % d;
    }
    assert(rem == 0);          /* division is exact */
    while (*len > 1 && limbs[*len - 1] == 0)
        --(*len);              /* strip leading zeros */
}

static inline void mul_uint64(uint64_t *limbs, size_t *len, uint64_t m)
{
    unsigned __int128 carry = 0;
    for (size_t i = 0; i < *len; ++i) {
        unsigned __int128 prod = (unsigned __int128)limbs[i] * m + carry;
        limbs[i] = (uint64_t)prod;
        carry   = prod >> 64;
    }
    while (carry) {
        limbs[*len] = (uint64_t)carry;
        carry >>= 64;
        ++(*len);
    }
}

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
