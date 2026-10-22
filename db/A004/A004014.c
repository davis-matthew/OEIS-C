/*
 * A004014  Norms of vectors in the b.c.c. lattice.
 * https://oeis.org/A004014
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   sieve-free scan: m is a term iff m == 3 (mod 8), or m == 4k with
 *            k not of the form 4^i*(8j+7) (k = 0 included)
 * Range:     u8 n <= 85, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   in_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/in_seq.h"

seq_t a(idx_t n) {
    uint64_t count = 0;
    uint64_t m = 0;
    for (;;) {
        if (in_seq(m)) {
            if (count == n) return (seq_t)m;
            count++;
        }
        m++;
    }
}
