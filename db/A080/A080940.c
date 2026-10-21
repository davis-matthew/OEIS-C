/*
 * A080940  Smallest proper divisor of n which is a suffix of n in binary
 *          representation; a(n) = 0 if no such divisor exists.
 * https://oeis.org/A080940
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = smallest proper divisor d of n such that the binary
 *            representation of d appears as a suffix of the binary
 *            representation of n; return 0 if no such divisor exists
 * Range:     u8 n <= 767, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Compute floor(log2(v)) for v>0 */
static inline unsigned int floor_log2_uint64(idx_t v) {
    unsigned int r = 0;
    while (v >>= 1) ++r;
    return r;
}

/* True if d is a proper divisor of n and the binary bits of d
   match the low-order bits of n. */
static inline bool suffix_ok(idx_t n, idx_t d) {
    if (d == 0 || d >= n) return false;
    unsigned int bits = floor_log2_uint64(d) + 1U;    /* number of bits in d */
    if (bits == 64) return false;                    /* cannot match if d has 64 bits */
    idx_t mask = (((idx_t)1u) << bits) - 1u;
    return (n & mask) == d;
}

extern seq_t a(idx_t n) {
    if (n <= 2) return (seq_t)0;        /* a(1)=0, a(2)=0 */

    idx_t min_d = 0;

    /* always check divisor 1 first */
    if ((n & 1u) == 1u) {
        min_d = 1;
        return (seq_t)1;   /* 1 is the smallest possible */
    }

    /* Enumerate divisors up to sqrt(n) */
    idx_t limit = (idx_t)floor_log2_uint64(n);
    for (idx_t i = 2; i <= (idx_t)((1u << limit) - 1u); ++i) {
        if (n % i == 0) {
            /* divisor i */
            if (suffix_ok(n, i) && (min_d == 0 || i < min_d))
                min_d = i;
            idx_t j = n / i;
            if (j != i && j < n) {          /* complementary divisor */
                if (suffix_ok(n, j) && (min_d == 0 || j < min_d))
                    min_d = j;
            }
        }
    }

    if (min_d == 0) return (seq_t)0;
    return (seq_t)min_d;
}
