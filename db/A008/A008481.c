/*
 * A008481  If n = Product (p_j^k_j) then a(n) = Sum partition(k_j).
 * https://oeis.org/A008481
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9217, u16 n <= 9217, u32 n <= 9217, u64 n <= 9217,
 *            u128 n <= 9217, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A008481
   Formula: a(n) = Σ partition(k_j), where n = ∏ p_j^k_j is the prime
   factorisation of n.  The function uses the provided prime‑factorisation
   routine factor_u64. */

/* The required helper is declared in the build utilities. */
#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

static seq_t partition_vals[65];
static bool partition_inited = false;

/* Initialise the partition table once.  Uses the pentagonal‑number
   recurrence. */
static void init_partition(void)
{
    partition_vals[0] = (seq_t)1;
    for (int n = 1; n <= 64; ++n) {
        int64_t sum = 0;
        for (int k = 1; ; ++k) {
            int g1 = k * (3 * k - 1) / 2;
            int g2 = k * (3 * k + 1) / 2;
            if (g1 > n && g2 > n) break;
            int sign = (k & 1) ? 1 : -1;
            if (g1 <= n) sum += sign * (int64_t)partition_vals[n - g1];
            if (g2 <= n) sum += sign * (int64_t)partition_vals[n - g2];
        }
        partition_vals[n] = (seq_t)sum;
    }
    partition_inited = true;
}

/* Compute a(n) for any n >= 1. */
seq_t a(idx_t n)
{
    if (!partition_inited) {
        init_partition();
    }

    if (n <= 1) {          /* a(1) = 0, n < 1 never queried in tests */
        return (seq_t)0;
    }

    /* The maximum number of distinct prime factors of a 64‑bit integer
       is less than 8, so 8 slots are ample. */
    uint64_t primes[8];
    uint64_t exps[8];
    size_t cnt = 0;

    /* Factorise n; factor_u64 is provided by the framework. */
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t sum = (seq_t)0;
    for (size_t i = 0; i < cnt; ++i) {
        /* exps[i] <= 64 for any 64‑bit n */
        sum += partition_vals[(int)exps[i]];
    }
    return sum;
}
