/*
 * A171806  Number of 5 X 5 permutation matrices such that the n-th matrix
 *          power is the least nonnegative power that gives the identity
 *          matrix.
 * https://oeis.org/A171806
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = number of permutations in S_5 of order n
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Validated: full, pass, 6 terms through n = 6 (2026-11-02)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* helper: factorial for small values (n <= 5) */
static seq_t fact_small(idx_t n)
{
    seq_t f = 1;
    for (idx_t i = 2; i <= n; ++i) {
        f *= (seq_t)i;
    }
    return f;
}

/* compute number of permutations of a given cycle type
   cycle[5] holds the cycle lengths, zeros allowed */
static seq_t count_type(const idx_t cycle[5])
{
    /* multiplicity of cycle lengths */
    idx_t freq[6] = {0}; /* indices 1..5 used */
    for (int i = 0; i < 5; ++i) {
        if (cycle[i] > 0) {
            freq[cycle[i]] += 1;
        }
    }

    /* denominator: product over lengths of (freq[l]! * l^freq[l]) */
    seq_t denom = 1;
    for (int l = 1; l <= 5; ++l) {
        if (freq[l] == 0) continue;
        /* freq[l]! */
        denom *= fact_small(freq[l]);

        /* l^freq[l] */
        seq_t pw = 1;
        for (idx_t k = 0; k < freq[l]; ++k) {
            pw *= (seq_t)l;
        }
        denom *= pw;
    }

    /* total factorial of 5 */
    static seq_t total_fact = 0;
    if (total_fact == 0) {
        total_fact = fact_small(5); /* 120 */
    }

    return total_fact / denom;
}

/* helper: gcd for idx_t */
static idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* helper: compute lcm for idx_t */
static idx_t lcm_idx(idx_t a, idx_t b)
{
    return (a / gcd_idx(a, b)) * b;
}

/* helper: compute order (lcm of cycle lengths) for a cycle type */
static idx_t order_type(const idx_t cycle[5])
{
    idx_t lcm = 1;
    for (int i = 0; i < 5; ++i) {
        if (cycle[i] > 0) {
            lcm = lcm_idx(lcm, cycle[i]);
        }
    }
    return lcm;
}

seq_t a(idx_t n)
{
    /* static initialization of order counts */
    static seq_t order_counts[7] = {0}; /* index by order 0..6 */
    static bool initialized = false;

    if (!initialized) {
        /* list of all cycle types of size 5 (up to 5 cycles) */
        const idx_t cycle_types[7][5] = {
            {1,1,1,1,1}, /* identity */
            {2,1,1,1},   /* one 2-cycle */
            {2,2,1},     /* two 2-cycles */
            {3,1,1},     /* one 3-cycle */
            {3,2},       /* 3-cycle + 2-cycle */
            {4,1},       /* one 4-cycle */
            {5}          /* one 5-cycle */
        };

        /* compute counts and orders for each type, accumulate per order */
        for (int i = 0; i < 7; ++i) {
            idx_t ord = order_type(cycle_types[i]);
            seq_t cnt = count_type(cycle_types[i]);

            if (ord <= 6) {
                order_counts[ord] += cnt;
            }
        }
        initialized = true;
    }

    if (n <= 6) {
        return order_counts[n];
    }
    return 0;
}
