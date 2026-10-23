/*
 * A162719  A positive integer n is included if n in binary contains as many
 *          0's as the number of 1's in the binary representation of n+1, and
 *          if n in binary contains as many 1's as the number of 0's in the
 *          binary representation of n+1.
 * https://oeis.org/A162719
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 38, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A162719: a(n) is the nth positive integer such that
   the number of 0-bits in n equals the number of 1-bits in n+1,
   and the number of 1-bits in n equals the number of 0-bits in n+1.
   The function computes the value by brute‑force scanning of candidates. */

#include <stdbool.h>
#include <stdint.h>

/* Count the number of 1-bits in a 64‑bit integer. */
static inline unsigned popcount_u64(uint64_t x)
{
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_popcountll((unsigned long long)x);
#else
    /* Portable fallback (very slow, used only if no builtin available). */
    unsigned cnt = 0;
    while (x) { cnt += (x & 1); x >>= 1; }
    return cnt;
#endif
}

/* Return the number of bits needed to represent x (x>0). */
static inline unsigned bitlen_u64(uint64_t x)
{
#if defined(__GNUC__) || defined(__clang__)
    return 64 - (unsigned)__builtin_clzll((unsigned long long)x);
#else
    unsigned len = 0;
    while (x) { len++; x >>= 1; }
    return len;
#endif
}

/* Test whether integer x satisfies the A162719 condition. */
static inline bool satisfies_a162719_u64(uint64_t x)
{
    uint64_t y = x + 1ULL;
    unsigned ones_x  = popcount_u64(x);
    unsigned ones_y  = popcount_u64(y);
    unsigned bits_x  = bitlen_u64(x);
    unsigned bits_y  = bitlen_u64(y);
    unsigned zeros_x = bits_x - ones_x;
    unsigned zeros_y = bits_y - ones_y;
    return (zeros_x == ones_y) && (ones_x == zeros_y);
}

/* External function expected by the build. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined, but return 0 for safety */

    idx_t seen = 0;
    uint64_t candidate = 1;

    while (1) {
        if (satisfies_a162719_u64(candidate)) {
            seen++;
            if (seen == n) {
                return (seq_t)candidate;
            }
        }
        candidate++;  /* next integer to test */
    }
}
