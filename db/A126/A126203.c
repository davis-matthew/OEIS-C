/*
 * A126203  Middle number of a set of 5 consecutive integers whose sum of
 *          cubes is a square.
 * https://oeis.org/A126203
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A126203: middle number of a set of 5 consecutive integers whose sum
   of cubes is a square.  a(n) is the nth integer n ≥ 0 for which
   5·n·(n²+6) is a perfect square. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

static inline uint64_t isqrt_uint64(uint64_t n) {
    uint64_t r = 0;
    for (int i = 63; i >= 0; --i) {
        uint64_t bit = 1ULL << i;
        uint64_t tmp = r | bit;
        if (tmp != 0 && tmp <= n / tmp)
            r = tmp;
    }
    return r;
}

static inline bool is_square_uint64(uint64_t n) {
    uint64_t s = isqrt_uint64(n);
    return s * s == n;
}

static seq_t sol[16];
static size_t sol_count = 0;
static bool initialized = false;

static void init_solutions(void) {
    if (initialized) return;
    const uint64_t LIMIT = 2000;   /* sufficient to find all known solutions */
    for (uint64_t m = 0; m <= LIMIT; ++m) {
        uint64_t val = 5 * m * (m * m + 6);
        if (is_square_uint64(val)) {
            if (sol_count < sizeof(sol) / sizeof(*sol))
                sol[sol_count++] = (seq_t)m;
        }
    }
    initialized = true;
}

seq_t a(idx_t n) {
    if (!initialized) init_solutions();
    if (n == 0) return 0;                /* offset is 1, but guard for safety */
    if ((idx_t)(n - 1) >= sol_count) return 0; /* undefined beyond known terms */
    return sol[n - 1];
}
