/*
 * A054868  Sum of bits of sum of bits of n: a(n) = wt(wt(n)).
 * https://oeis.org/A054868
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = wt(wt(n)) (number of 1‑bits of the number of 1‑bits of n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Count the popcount of an unsigned 64‑bit integer */
static inline int popcnt_u64(uint64_t x) {
    /* Use compiler built‑in if available, otherwise fallback to bit‑shifting */
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_popcountll(x);
#else
    int c = 0;
    while (x) {
        c += (int)(x & 1U);
        x >>= 1;
    }
    return c;
#endif
}

seq_t a(idx_t n) {
    /* First popcount: number of 1 bits in n */
    int first = popcnt_u64((uint64_t)n);

    /* Second popcount: number of 1 bits in that count */
    int second = popcnt_u64((uint64_t)first);

    return (seq_t)second;
}
