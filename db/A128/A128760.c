/*
 * A128760  Number of ways to write n as the absolute difference of a power of
 *          2 and a power of 3.
 * https://oeis.org/A128760
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{(i,j) | |2^i – 3^j| = n }
 * Range:     u8 n <= 12, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

static seq_t pow2[200];
static seq_t pow3[200];
static size_t cnt2 = 0;
static size_t cnt3 = 0;
static bool initialized = false;

seq_t a(idx_t n) {
    /* One‑time generation of all powers of 2 and 3 that fit in seq_t */
    if (!initialized) {
        seq_t val = 1;
        /* powers of 2 */
        for (cnt2 = 0; cnt2 < 200; ++cnt2) {
            pow2[cnt2] = val;
            seq_t next = val * 2;
            if (next < val)  /* overflow detection */
                break;
            val = next;
        }
        /* powers of 3 */
        val = 1;
        for (cnt3 = 0; cnt3 < 200; ++cnt3) {
            pow3[cnt3] = val;
            seq_t next = val * 3;
            if (next < val)  /* overflow detection */
                break;
            val = next;
        }
        initialized = true;
    }

    seq_t target = (seq_t)n;
    seq_t count = 0;

    for (size_t i = 0; i < cnt2; ++i) {
        seq_t p2 = pow2[i];
        for (size_t j = 0; j < cnt3; ++j) {
            seq_t p3 = pow3[j];
            seq_t diff = (p2 > p3) ? (p2 - p3) : (p3 - p2);
            if (diff == target) {
                ++count;
            }
        }
    }
    return count;
}
