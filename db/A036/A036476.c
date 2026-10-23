/*
 * A036476  a(n) = partition(11n+2) mod 11.
 * https://oeis.org/A036476
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = partition(11*n + 2) mod 11 OEIS sequence A036476. The
 *            function uses the Euler pentagonal‑number recurrence to build
 *            partition numbers modulo 11 on demand. Results are cached in a
 *            static array so that repeated calls are O(1) after the first
 *            evaluation. The implementation works for any seq_t...
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <assert.h>

#define MAX_PARTITIONS 120000            /* enough for n up to at least 10000 */

static seq_t partition_cache[MAX_PARTITIONS];
static idx_t partition_cache_size = 0;

/* Ensure partition_cache[0..n] (mod 11) have been computed. */
static void ensure_partition(idx_t n)
{
    if (n < partition_cache_size)
        return;

    if (n >= MAX_PARTITIONS) {
        /* Out of range: abort on conservative builds. */
        assert(0 && "n too large for static cache");
    }

    if (partition_cache_size == 0) {
        partition_cache[0] = (seq_t)1;
        partition_cache_size = 1;
    }

    for (idx_t i = partition_cache_size; i <= n; ++i) {
        int sum = 0;
        for (idx_t k = 1; ; ++k) {
            idx_t t1 = k * (3 * k - 1) / 2;
            if (t1 > i)
                break;
            int term = (int)partition_cache[i - t1];
            int sign = (k & 1) ? 1 : -1;
            sum += sign * term;

            idx_t t2 = k * (3 * k + 1) / 2;
            if (t2 <= i) {
                term = (int)partition_cache[i - t2];
                sum += sign * term;
            }
        }
        int mod = sum % 11;
        if (mod < 0)
            mod += 11;
        partition_cache[i] = (seq_t)mod;
    }
    partition_cache_size = n + 1;
}

seq_t a(idx_t n)
{
    idx_t target = 11 * n + 2;
    ensure_partition(target);
    return partition_cache[target];
}
