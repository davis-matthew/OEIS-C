/*
 * A124069  Fixed points for operation of repeatedly replacing a number with
 *          the sum of the eighth power of its digits.
 * https://oeis.org/A124069
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A124069 – fixed points of the operation “replace a number by the sum of the eighth powers of its digits”. */
/* The routine builds the list of all fixed points in the range [0, max_sum] once,
   then returns the nth element (1‑based). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Pre‑computed eighth powers of the digits 0–9. */
static uint32_t pow8[10];

/* Sum of eighth powers for all numbers 0–999999. */
static uint32_t *sum8 = NULL;

/* Array of found fixed points. */
static seq_t *fixed = NULL;
static size_t fixed_len = 0;
static size_t fixed_cap = 0;

/* Helper to ensure the auxiliary tables are built. */
static bool build_tables(void) {
    if (sum8 != NULL) return true;        /* already built */

    /* Compute digit^8 values. */
    for (int d = 0; d <= 9; ++d) {
        uint64_t p = 1;
        for (int i = 0; i < 8; ++i) p *= (uint64_t)d;
        pow8[d] = (uint32_t)p;           /* fits in 32 bits */
    }

    /* Allocate array for all 6‑digit numbers (0..999999). */
    const uint32_t SZ = 1000000U;
    sum8 = (uint32_t*)malloc(SZ * sizeof(uint32_t));
    if (!sum8) return false;

    /* Compute sum of eighth powers for each entry. */
    for (uint32_t i = 0; i < SZ; ++i) {
        uint32_t x = i;
        uint32_t s = 0;
        do {
            s += pow8[x % 10];
            x /= 10;
        } while (x);
        sum8[i] = s;
    }

    return true;
}

/* Helper to append a new fixed point to our list. */
static void append_fixed(uint32_t val) {
    if (fixed_len == fixed_cap) {
        size_t new_cap = fixed_cap ? fixed_cap * 2 : 64;
        seq_t *tmp = (seq_t*)realloc(fixed, new_cap * sizeof(seq_t));
        if (!tmp) return;            /* out of memory – abort silently */
        fixed = tmp;
        fixed_cap = new_cap;
    }
    fixed[fixed_len++] = (seq_t)val;
}

/* Build the list of fixed points once. */
static void build_fixed_points(void) {
    if (fixed_len > 0) return;          /* already built */

    if (!build_tables()) return;        /* failed to build tables */

    /* The theoretical upper bound for a fixed point is 9 * 9^8 = 387420489. */
    const uint32_t MAX_SUM = 387420489U;
    const uint32_t PART = 1000000U;      /* 10^6 */

    for (uint32_t hi = 0; ; ++hi) {
        uint32_t base = hi * PART;
        if (base > MAX_SUM) break;
        uint32_t lo_end = 999999U;
        if (base + lo_end > MAX_SUM) lo_end = MAX_SUM - base;
        for (uint32_t lo = 0; lo <= lo_end; ++lo) {
            uint32_t n = base + lo;
            uint32_t s = sum8[hi] + sum8[lo];
            if (s == n) append_fixed(n);
        }
    }
}

/* Public function required by the build. */
seq_t a(idx_t n) {
    if (n == 0) return 0;                /* out of range below offset */

    if (fixed_len == 0) build_fixed_points();

    if ((uint64_t)n > fixed_len) return 0;/* out of range */

    return fixed[n - 1];
}
