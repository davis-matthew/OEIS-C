/*
 * A145770  a(n) = number of elements of order n in simple group Alt(10) of
 *          order 1814400.
 * https://oeis.org/A145770
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = number of elements of order n in Alt(10)
 * Range:     u8 none, u16 n <= 1, u32 n <= 21, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* Helper: gcd for seq_t */
static seq_t _gcd_seq(seq_t a, seq_t b) {
    seq_t temp;
    while (b != 0) {
        temp = a % b;
        a = b;
        b = temp;
    }
    return a;
}

/* Helper: lcm of two seq_t */
static seq_t _lcm_seq(seq_t a, seq_t b) {
    if (a == 0 || b == 0) return 0;
    return (a / _gcd_seq(a, b)) * b;
}

/* Recursive generation of integer partitions of 10 */
static void _gen_partitions(int remaining, int max_part, int *parts,
                            int depth, seq_t *counts, seq_t fact) {
    if (remaining == 0) {
        int len = depth;
        /* parity check: (10 - len) must be even for a even permutation */
        if (((10 - len) & 1) == 0) {
            /* compute lcm of parts */
            seq_t order = 1;
            for (int i = 0; i < len; ++i)
                order = _lcm_seq(order, (seq_t)parts[i]);

            /* compute denominator: product over i (m_i! * i^{m_i}) */
            seq_t denom = 1;
            /* Count multiplicities of each part size */
            int i = 0;
            while (i < len) {
                int val = parts[i];
                int cnt = 0;
                while (i < len && parts[i] == val) {
                    ++cnt;
                    ++i;
                }
                /* multiply by cnt! */
                seq_t fac = 1;
                for (int k = 2; k <= cnt; ++k) fac *= k;
                denom *= fac;
                /* multiply by val^cnt */
                seq_t pw = 1;
                for (int k = 0; k < cnt; ++k) pw *= (seq_t)val;
                denom *= pw;
            }
            seq_t count = fact / denom;
            if (order < 61) counts[(idx_t)order] += count;
        }
        return;
    }
    for (int p = (remaining < max_part) ? remaining : max_part; p >= 1; --p) {
        parts[depth] = p;
        _gen_partitions(remaining - p, p, parts, depth + 1, counts, fact);
    }
}

seq_t a(idx_t n) {
    /* static cache for counts */
    static bool initialized = false;
    static seq_t counts[61];
    if (!initialized) {
        /* initialize counts to zero */
        for (idx_t i = 0; i < 61; ++i) counts[i] = 0;
        /* factorial 10! */
        seq_t fact = 1;
        for (int i = 2; i <= 10; ++i) fact *= (seq_t)i;
        int parts[10];
        _gen_partitions(10, 10, parts, 0, counts, fact);
        initialized = true;
    }
    if (n < 61) return counts[(idx_t)n];
    return 0;
}
