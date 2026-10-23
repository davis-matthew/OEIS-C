/*
 * A043741  Numbers whose number of runs in the base-2 representation is
 *          congruent to 2 mod 7.
 * https://oeis.org/A043741
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n): the n-th positive integer whose number of runs
 * in the binary representation is congruent to 2 modulo 7.
 * The runs are maximal consecutive equal bits in the binary string
 * without leading zeros. */

#include <stdint.h> /* for uint64_t if needed */

static inline idx_t run_count(idx_t v) {
    /* Count runs in binary representation of v (v ≥ 1). */
    if (v == 0) return 1;          /* not used, but safe */
    idx_t tmp = v;
    int runs = 1;                  /* at least one run */
    int prev = tmp & 1;            /* least significant bit */
    tmp >>= 1;
    while (tmp) {
        int bit = tmp & 1;
        if (bit != prev) runs++;
        prev = bit;
        tmp >>= 1;
    }
    return (idx_t)runs;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* offset is 1, but guard for safety */

    idx_t val = 2;                 /* first candidate (2 = 10b) */
    idx_t count = 0;

    while (count < n) {
        if (run_count(val) % 7 == 2) {
            count++;
            if (count == n) return (seq_t)val;
        }
        val++;
    }

    /* unreachable */
    return (seq_t)0;
}
