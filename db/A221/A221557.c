/*
 * A221557  Consecutive values produced by the C++ mt19937 (Mersenne twister)
 *          random number generator with the default seed (5489).
 * https://oeis.org/A221557
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth output of the C++ mt19937 RNG seeded with 5489
 * Range:     u8 none, u16 none, u32 n <= 1000, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term (n ≥ 1) of OEIS sequence A221557,
   i.e. the nth value produced by the C++ mt19937 random number
   generator with its default seed 5489.  The computation is
   independent of any previous calls and works in any order of n. */

#include <stdint.h>
#include <stddef.h>

/* C++ mt19937 constants */
#define MT19937_N   624
#define MT19937_M   397
#define MT19937_MATRIX_A 0x9908b0dfu
#define MT19937_UPPER_MASK 0x80000000u
#define MT19937_LOWER_MASK 0x7fffffffu

/* Seed used by the C++ standard library */
#define MT19937_DEFAULT_SEED 5489u

static void mt19937_seed(uint32_t state[MT19937_N]) {
    state[0] = MT19937_DEFAULT_SEED;
    for (size_t i = 1; i < MT19937_N; ++i) {
        state[i] = (1812433253u *
                        (state[i - 1] ^ (state[i - 1] >> 30)) + i);
    }
}

static void mt19937_twist(uint32_t state[MT19937_N]) {
    for (size_t i = 0; i < MT19937_N; ++i) {
        uint32_t x = (state[i] & MT19937_UPPER_MASK) |
                     (state[(i + 1) % MT19937_N] & MT19937_LOWER_MASK);
        state[i] = state[(i + MT19937_M) % MT19937_N] ^ (x >> 1);
        if (x & 1u) {
            state[i] ^= MT19937_MATRIX_A;
        }
    }
}

static uint32_t mt19937_extract(uint32_t state[MT19937_N], size_t *index) {
    if (*index >= MT19937_N) {
        mt19937_twist(state);
        *index = 0;
    }
    uint32_t y = state[(*index)++];
    /* Tempering */
    y ^= (y >> 11);
    y ^= (y << 7) & 0x9d2c5680u;
    y ^= (y << 15) & 0xefc60000u;
    y ^= (y >> 18);
    return y;
}

seq_t a(idx_t n) {
    /* n is 1‑based; the function is called only for n ≥ 1. */
    uint32_t mt_state[MT19937_N];
    size_t mt_index = MT19937_N;  /* so that the first call twists */
    mt19937_seed(mt_state);

    uint32_t result = 0;
    for (idx_t i = 1; i <= n; ++i) {
        result = mt19937_extract(mt_state, &mt_index);
    }
    return (seq_t)result;
}
