/*
 * A145754  a(n) = number of elements of order n in simple group Alt(9) of
 *          order 181440.
 * https://oeis.org/A145754
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count elements of order n in Alt(9) by enumerating cycle types */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

/* multiply a base by itself exp times */
static seq_t pow_small_seq(seq_t base, unsigned int exp)
{
    seq_t r = 1;
    for (unsigned int i = 0; i < exp; ++i)
        r *= base;
    return r;
}

/* factorials up to 9 */
static seq_t fact[10];
#define MAX_ORDER 2520
/* table holding the result for each order up to the maximum */
static seq_t counts[MAX_ORDER + 1];

/* recursive partition generator: give all ways to write rem as a sum of ≤ max_part */
static void generate_partitions(int rem, int max_part, int *parts, int len)
{
    if (rem == 0) {
        /* count how many cycles of each length we have */
        int m[10] = {0};
        for (int i = 0; i < len; ++i)
            m[parts[i]]++;

        /* total number of cycles (1‑cycles included) */
        int total_cycles = 0;
        for (int k = 1; k <= 9; ++k)
            total_cycles += m[k];

        /* a permutation is even iff the number of cycles is odd (9 is odd) */
        if ((total_cycles & 1) == 1) {
            /* lcm of the cycle lengths */
            seq_t lcm = 1;
            for (int k = 1; k <= 9; ++k) {
                if (m[k] > 0) {
                    seq_t g = gcd(lcm, (seq_t)k);
                    lcm = (lcm * (seq_t)k) / g;
                }
            }

            /* size of the conjugacy class in S9 */
            seq_t denom = 1;
            for (int k = 1; k <= 9; ++k) {
                if (m[k] > 0) {
                    seq_t pk = pow_small_seq((seq_t)k, (unsigned int)m[k]);
                    denom *= pk * fact[m[k]];
                }
            }
            seq_t class_size = fact[9] / denom;

            /* add the count to the appropriate order */
            idx_t idx = (idx_t)lcm;
            if (idx <= MAX_ORDER)
                counts[idx] += class_size;
        }
        return;
    }

    for (int i = ((rem < max_part) ? rem : max_part); i >= 1; --i) {
        parts[len] = i;
        generate_partitions(rem - i, i, parts, len + 1);
    }
}

/* one‑time initialization of the precomputation */
static bool initialized = false;
static void init_counts(void)
{
    if (initialized)
        return;
    initialized = true;

    fact[0] = 1;
    for (int i = 1; i <= 9; ++i)
        fact[i] = fact[i - 1] * (seq_t)i;

    for (int i = 0; i <= MAX_ORDER; ++i)
        counts[i] = 0;

    int parts[9];
    generate_partitions(9, 9, parts, 0);
}

/* required public function */
seq_t a(idx_t n)
{
    init_counts();
    if (n <= MAX_ORDER)
        return counts[(unsigned)n];
    return 0;
}
