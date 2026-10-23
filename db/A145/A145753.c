/*
 * A145753  a(n) = number of elements of order n in simple group Alt(8) of
 *          order 20160.
 * https://oeis.org/A145753
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = Σ 8! / ∏ i^{m_i}·m_i! over all cycle types of S₈ * with
 *            an even number of cycles and lcm == n (all such permutations are
 *            * even, hence belong to Alt(8))
 * Range:     u8 n <= 1, u16 n <= 15, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/gcd.h"

static bool          initialized = false;
static seq_t order_counts[16] = {0};

static seq_t pow_seq(seq_t base, int exp)
{
    seq_t r = 1;
    for (int i = 0; i < exp; ++i)
        r *= base;
    return r;
}

static void generate_partition(int max, int remaining, int parts[],
                               int depth, seq_t fact8, seq_t fact[9])
{
    if (remaining == 0) {
        /* even number of cycles? */
        if ((depth & 1) != 0) return;

        /* multiplicities of cycle lengths */
        int mult[9] = {0};
        for (int i = 0; i < depth; ++i)
            ++mult[parts[i]];

        /* compute lcm of cycle lengths */
        seq_t order = 1;
        for (int i = 0; i < depth; ++i) {
            int len = parts[i];
            seq_t g   = gcd(order, (seq_t)len);
            order = (order / g) * (seq_t)len;
        }

        /* count permutations of this cycle type */
        seq_t count = fact8;
        for (int len = 1; len <= 8; ++len) {
            int m = mult[len];
            if (m > 0) {
                seq_t denom = pow_seq((seq_t)len, m);
                denom *= fact[m];
                count /= denom;
            }
        }

        if (order <= 15) {                     /* 15 is the largest possible order */
            order_counts[(size_t)order] += count;
        }
        return;
    }

    for (int i = max; i >= 1; --i) {
        if (i > remaining) continue;
        parts[depth] = i;
        generate_partition(i, remaining - i, parts, depth + 1, fact8, fact);
    }
}

static void init(void)
{
    if (initialized) return;

    /* factorials 0! .. 8! */
    seq_t fact[9];
    fact[0] = 1;
    for (int i = 1; i <= 8; ++i)
        fact[i] = fact[i - 1] * (seq_t)i;

    seq_t fact8 = fact[8];
    int   parts[8];

    generate_partition(8, 8, parts, 0, fact8, fact);
    initialized = true;
}

seq_t a(idx_t n)
{
    if (!initialized)
        init();

    if (n <= 15)
        return order_counts[(size_t)n];
    return 0;
}
