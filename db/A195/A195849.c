/*
 * A195849  Column 5 of array A195825. Also column 1 of triangle A195839. Also
 *          1 together with the row sums of triangle A195839.
 * https://oeis.org/A195849
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into parts congruent to 0, 1 or
 *            6 modulo 7
 * Range:     u8 n <= 40, u16 n <= 103, u32 n <= 302, u64 n <= 978,
 *            u128 n <= 3414, big any n
 * Validated: standard, pass, 77 terms through n = 9944 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses a classic “coin‐change” DP.  The array of
   dp[0..n] is allocated with alloc_seq_array, which guarantees proper
   initialization of each seq_t element.  All arithmetic is done with
   the provided seq_t type, so the code works for 8,16,32,64,128‑bit
   unsigned integers and for the arbitrary‑precision build.  */

#include <stddef.h>               /* for size_t */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

static seq_t *alloc_seq_array(size_t n)
{
    return mem_new(seq_t, n);
}

static void free_seq_array(seq_t *p)
{
    mem_free(p);
}

seq_t a(idx_t n)
{
    /* For n = 0 the answer is 1 (empty partition). */
    if (n == 0U) {
        return 1;
    }

    /* Allocate dp[0..n] with proper zeroing. */
    size_t sz = (size_t)n + 1U;
    seq_t *dp = alloc_seq_array(sz);
    for (size_t i = 0; i < sz; ++i) {
        dp[i] = 0;
    }
    dp[0] = 1;   /* base case */

    /* Enumerate all allowed part sizes: 7k, 7k-1, 7k-6 (k ≥ 1). */
    for (idx_t k = 1; ; ++k) {
        idx_t p0 = 7 * k;          /* ≡ 0 (mod 7) */
        idx_t p6 = 7 * k - 1;      /* ≡ 6 (mod 7) */
        idx_t p1 = 7 * k - 6;      /* ≡ 1 (mod 7) */

        /* Stop when all three exceed n. */
        if (p0 > n && p6 > n && p1 > n) {
            break;
        }

        if (p0 <= n) {
            for (idx_t i = p0; i <= n; ++i) {
                dp[i] += dp[i - p0];
            }
        }
        if (p6 <= n) {
            for (idx_t i = p6; i <= n; ++i) {
                dp[i] += dp[i - p6];
            }
        }
        if (p1 <= n) {
            for (idx_t i = p1; i <= n; ++i) {
                dp[i] += dp[i - p1];
            }
        }
    }

    seq_t result = dp[n];
    free_seq_array(dp);
    return result;
}
