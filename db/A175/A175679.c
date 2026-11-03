/*
 * A175679  Numbers m such that arithmetic mean Ad(m) of divisors of m and
 *          arithmetic mean Ak(m) of numbers 1 <= k <= m are both integer.
 * https://oeis.org/A175679
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th odd arithmetic number, i.e. the n-th m such that
 *            m is odd and the average of its divisors is an integer
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Validated: partial, pass, 64 terms through n = 64 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Header inclusions */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Helper to compute whether a positive integer m is an arithmetic number
   (i.e. sigma(m)/tau(m) is an integer). Only 64‑bit integers are needed
   for the expected range (m ≈ 20000 when n ≤ 10000). */
static bool is_arithmetic_number(uint64_t m)
{
    uint64_t temp = m;
    uint64_t sigma = 1;        /* product of divisor sums of prime powers */
    uint64_t tau = 1;          /* product of (exponent+1) for each prime */

    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            /* count exponent of prime p */
            uint64_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            /* compute sum of powers of p: 1 + p + p^2 + ... + p^exp */
            uint64_t sum = 1;
            uint64_t power = 1;
            for (uint64_t i = 0; i < exp; ++i) {
                power *= p;
                sum += power;
            }
            sigma *= sum;
            tau  *= (exp + 1);
        }
    }
    /* remaining prime factor, if any */
    if (temp > 1) {
        sigma *= (1 + temp);
        tau  *= 2;
    }
    return sigma % tau == 0;
}

/* Main sequence function.  Implements memoization so repeated or out-of-
   order calls stay efficient. */
seq_t a(idx_t n)
{
    /* offset is 1, so a(1) = 1 */
    static seq_t *cache = NULL;      /* cached sequence values */
    static idx_t  cache_len = 0;     /* allocated size of cache */
    static idx_t  found = 0;         /* number of terms already found */
    static uint64_t next_m = 1;      /* next candidate integer to test */

    if (n == 0) return 0;            /* safety: offset is 1 */

    /* Ensure cache is large enough for the requested index */
    if (cache_len < n) {
        idx_t new_len = (n < 16 ? n + 16 : n * 2);
        seq_t *new_cache = mem_new(seq_t, new_len);
        if (!new_cache) return 0;    /* allocation failure; unlikely */
        if (cache) mem_free(cache);
        cache = new_cache;
        cache_len = new_len;
    }

    /* Generate terms until we have at least n */
    while (found < n) {
        /* Skip even numbers */
        if (next_m & 1) {
            if (is_arithmetic_number(next_m)) {
                cache[found] = (seq_t)next_m;
                ++found;
            }
        }
        ++next_m;
    }

    return cache[n - 1];
}
