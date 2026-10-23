/*
 * A162400  a(n) = the largest square that when represented in binary is a
 *          substring within the binary representation of n.
 * https://oeis.org/A162400
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the largest square whose binary representation appears as
 *            a substring of the binary representation of n
 * Range:     u8 n <= 255, u16 n <= 4096, u32 n <= 4096, u64 n <= 4096,
 *            u128 n <= 4096, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Compute floor(sqrt(x)) for unsigned 64-bit integers */
static inline idx_t floor_sqrt_u64(idx_t x) {
    if (x < 2) return x;
    idx_t lo = 1, hi = ((idx_t)1 << 32);  /* sqrt(2^64-1) < 2^32 */
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) / 2;
        if (mid <= x / mid)  /* mid*mid <= x without overflow */
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* Number of bits needed to represent x (x > 0) */
static inline unsigned bitlen_uint64(idx_t x) {
    return 64 - __builtin_clzll(x);
}

seq_t a(idx_t n) {
    /* Handle trivial small cases directly */
    if (n == 1) return (seq_t)1;
    unsigned len_n = bitlen_uint64(n);

    idx_t max_root = floor_sqrt_u64(n);

    for (idx_t root = max_root; root >= 1; --root) {
        idx_t sq = root * root;          /* i*i <= n fits in 64 bits */
        unsigned len_sq = bitlen_uint64(sq);

        if (len_sq > len_n) continue;    /* cannot fit inside n */

        /* Prepare mask of len_sq bits */
        idx_t mask;
        if (len_sq == 64)
            mask = ~((idx_t)0);
        else
            mask = ((idx_t)1 << len_sq) - 1;

        /* Scan all possible bit positions where sq could appear in n */
        idx_t max_shift = len_n - len_sq;
        for (idx_t shift = 0; shift <= max_shift; ++shift) {
            if (((n >> shift) & mask) == sq)
                return (seq_t)sq;         /* found biggest square */
        }
    }

    /* Fallback – should never reach here as 1 is always a candidate */
    return (seq_t)1;
}
