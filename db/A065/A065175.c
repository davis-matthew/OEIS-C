/*
 * A065175  Inverse permutation to A065174.
 * https://oeis.org/A065175
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + δ(n) where δ(n) = 1 if n is a power of 2 -2^{v₂(n)+1}
 *            if n is even and not a power of 2 +2^{v₂(n-1)+1} if n is odd and
 *            n > 1 (v₂(x) is the exponent of 2 in the factorisation of x)
 *            a(1) = 1 This yields the full sequence A065175 (inverse of
 *            A065174)
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static inline bool is_power_of_two(idx_t n) {
    return n != 0 && (n & (n - 1)) == 0;
}

/* Return the exponent of 2 in the prime factorisation of x (x>=1). */
static inline unsigned v2_uint64(idx_t x) {
    unsigned v = 0;
    while ((x & 1ULL) == 0) {
        ++v;
        x >>= 1;
    }
    return v;
}

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)1;
    if (is_power_of_two(n)) return (seq_t)n + (seq_t)1;
    if ((n & 1ULL) == 0) {          /* even, not a power of two */
        unsigned v = v2_uint64(n);
        seq_t diff = ((seq_t)1ULL << (v + 1));
        return (seq_t)n - diff;
    } else {                         /* odd, n > 1 */
        unsigned v = v2_uint64(n - 1);
        seq_t diff = ((seq_t)1ULL << (v + 1));
        return (seq_t)n + diff;
    }
}
