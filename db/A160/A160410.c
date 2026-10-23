/*
 * A160410  Number of "ON" cells at n-th stage in simple 2-dimensional
 *          cellular automaton (see Comments for precise definition).
 * https://oeis.org/A160410
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = 4 * ∑_{k=0}^{n-1} 3^{popcount(k)}
 * Range:     u8 n <= 7, u16 n <= 127, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

static seq_t pow3[65];

/* Initialise powers of 3 once. */
static void init_pow3(void)
{
    static bool init = false;
    if (!init) {
        pow3[0] = (seq_t)1;
        for (unsigned i = 1; i < 65; ++i)
            pow3[i] = pow3[i-1] * (seq_t)3;
        init = true;
    }
}

/* Portable popcount for 64‑bit values. */
static inline unsigned popcount_u64(idx_t x)
{
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_popcountll((unsigned long long)x);
#else
    unsigned c = 0;
    while (x) {
        x &= x - 1;
        ++c;
    }
    return c;
#endif
}

seq_t a(idx_t n)
{
    init_pow3();
    seq_t sum = 0;
    for (idx_t k = 0; k < n; ++k) {
        unsigned pc = popcount_u64(k);
        sum += pow3[pc];
    }
    return sum * (seq_t)4;
}
