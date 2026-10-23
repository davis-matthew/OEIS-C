/*
 * A101872  Number of Abelian groups of order 2n.
 * https://oeis.org/A101872
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = number of abelian groups of order 2n = A000688(2n)
 * Range:     u8 n <= 8362, u16 n <= 8362, u32 n <= 8362, u64 n <= 8362,
 *            u128 n <= 8362, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* maximum exponent we anticipate for a prime factor of 2*n
   (for n up to 10000 the largest exponent is < 20) */
#define MAX_PART 200

/* partitions table, built on demand */
static int64_t part_int[MAX_PART];
static int part_max = -1;

/* ensure partitions up to e are computed */
static void ensure_partitions(int e) {
    if (e <= part_max) return;
    for (int i = part_max + 1; i <= e; ++i) {
        int64_t sum = 0;
        for (int k = 1; ; ++k) {
            int pent1 = k * (3 * k - 1) / 2;
            if (pent1 > i) break;
            sum += (k & 1) ? part_int[i - pent1] : -part_int[i - pent1];
            int pent2 = k * (3 * k + 1) / 2;
            if (pent2 > i) continue;
            sum += (k & 1) ? part_int[i - pent2] : -part_int[i - pent2];
        }
        part_int[i] = sum;
    }
    part_max = e;
}

/* return partition number p(e) */
static inline seq_t part(int e) {
    if (part_max < 0) { part_int[0] = 1; part_max = 0; }
    ensure_partitions(e);
    return (seq_t)part_int[e];
}

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* safe for completeness */
    uint64_t m = 2 * (uint64_t)n;   /* compute 2n */
    seq_t result = 1;

    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            int exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            result *= part(exp);
        }
    }

    if (m > 1) {                    /* remaining prime factor */
        result *= part(1);
    }

    return result;
}
