/*
 * A199014  Divisors of 196884.
 * https://oeis.org/A199014
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 11, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   cmp_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Divisors of 196884 in ascending order:
   a(n) is the n‑th divisor of 196884 (1‑based). */

/* This implementation computes all 24 divisors once,
   sorts them, and returns the nth element.  For n > 24
   a value of 0 is returned. */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/cmp_seq_t.h"

/* constant factorization of 196884 = 2^2 * 3^3 * 1823 */
static const uint64_t prime1 = 2;
static const uint64_t prime2 = 3;
static const uint64_t prime3 = 1823;
static const unsigned exp1   = 2;   /* exponent of prime1 */
static const unsigned exp2   = 3;   /* exponent of prime2 */
static const unsigned exp3   = 1;   /* exponent of prime3 */

static seq_t sorted_divisors[24];
static size_t divisor_count = 0;
static bool initialized = false;

static void init_divisors(void) {
    size_t idx = 0;
    for (unsigned e1 = 0; e1 <= exp1; ++e1) {
        seq_t p1 = 1;
        for (unsigned k = 0; k < e1; ++k) p1 *= (seq_t)prime1;
        for (unsigned e2 = 0; e2 <= exp2; ++e2) {
            seq_t p2 = p1;
            for (unsigned k = 0; k < e2; ++k) p2 *= (seq_t)prime2;
            for (unsigned e3 = 0; e3 <= exp3; ++e3) {
                seq_t val = p2;
                if (e3) val *= (seq_t)prime3;
                sorted_divisors[idx++] = val;
            }
        }
    }
    divisor_count = idx;                    /* should be 24 */
    qsort(sorted_divisors, divisor_count, sizeof(seq_t), cmp_seq_t);
    initialized = true;
}

seq_t a(idx_t n) {
    if (!initialized) init_divisors();
    if (n == 0) return 0;                 /* safety for accidental zero */
    if (n <= divisor_count) {
        return sorted_divisors[(size_t)(n - 1)];
    }
    return 0;            /* undefined for n > 24 */
}
