/*
 * A145437  a(n) = number of elements of order n in simple group Alt(12) of
 *          order 239500800.
 * https://oeis.org/A145437
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = number of elements of order n in Alt(12)
 * Range:     u8 none, u16 n <= 1, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* helper: Euclidean gcd for seq_t */
static seq_t gcd_seq_t(seq_t a, seq_t b) {
    while (b != 0) {
        seq_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

/* recursive enumeration of cycle-type partitions of 12 */
static void enumerate_partitions(int rem, int max_part, int depth,
                                 int *parts, seq_t *table, const seq_t *fact) {
    if (rem == 0) {
        int m[13] = {0};
        for (int i = 0; i < depth; ++i) m[parts[i]]++;

        /* compute lcm of cycle lengths */
        seq_t order = 1;
        for (int i = 0; i < depth; ++i) {
            seq_t l = (seq_t)parts[i];
            seq_t g = gcd_seq_t(order, l);
            order = (order / g) * l;
        }

        /* parity of number of cycles */
        int cycles = depth;
        if ((12 - cycles) % 2 == 0) { /* even permutation */
            /* count permutations of this type: 12! / (∏ i^{m_i} * m_i!) */
            seq_t denom = 1;
            for (int len = 1; len <= 12; ++len) {
                int mi = m[len];
                if (mi > 0) {
                    /* len^mi */
                    seq_t powv = 1;
                    for (int k = 0; k < mi; ++k) powv *= (seq_t)len;
                    denom *= powv;
                    /* mi! */
                    seq_t factmi = 1;
                    for (int k = 2; k <= mi; ++k) factmi *= (seq_t)k;
                    denom *= factmi;
                }
            }
            seq_t count = fact[12] / denom;
            idx_t idx = (idx_t)order;
            if (idx < 27721) table[idx] += count;
        }
        return;
    }

    for (int part = (rem < max_part) ? rem : max_part; part >= 1; --part) {
        parts[depth] = part;
        enumerate_partitions(rem - part, part, depth + 1, parts, table, fact);
    }
}

/* main sequence function */
seq_t a(idx_t n) {
    static seq_t *table = NULL;
    static bool initialized = false;
    const int LIM = 27721;            /* covers all possible orders */

    if (!initialized) {
        table = (seq_t*)malloc(sizeof(seq_t)*LIM);
        for (int i = 0; i < LIM; ++i) table[i] = 0;

        /* factorials up to 12! */
        seq_t fact[13];
        fact[0] = 1;
        for (int i = 1; i <= 12; ++i) fact[i] = fact[i-1] * (seq_t)i;

        int parts[12];
        enumerate_partitions(12, 12, 0, parts, table, fact);

        initialized = true;
    }

    if (n < LIM) return table[(idx_t)n];
    return (seq_t)0;
}
