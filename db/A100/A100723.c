/*
 * A100723  Prime numbers whose binary representations are split into exactly
 *          seven runs.
 * https://oeis.org/A100723
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the n-th prime whose binary representation has exactly
 *            seven runs of consecutive * equal bits. Computation is by
 *            brute‑force enumeration of primes, testing runs directly
 * Range:     u8 n <= 3, u16 n <= 2280, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bitlen, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses the helper functions from "../utils/bitlen.h" (bitlen) and
 * "../utils/is_prime.h" (is_prime) which are provided by the build system. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/bitlen.h"
#include "../utils/is_prime.h"

/* Count the number of runs of consecutive identical bits in x.
   The most significant bit is the first run. */
static int count_runs_uint64(uint64_t x)
{
    if (x == 0) return 1;   /* not used for primes > 1 */
    idx_t msb = bitlen(x) - 1;   /* highest set bit position */
    int runs = 1;
    int prev = (x >> msb) & 1;
    for (int i = msb - 1; i >= 0; --i) {
        int cur = (x >> i) & 1;
        if (cur != prev) {
            ++runs;
            prev = cur;
        }
    }
    return runs;
}

/* Return the n-th prime (n >= 1) whose binary representation splits into exactly seven runs. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n < 1, return 0 */

    idx_t found = 0;
    uint64_t candidate = 2;   /* start with the first prime */

    while (1) {
        if (is_prime(candidate) && count_runs_uint64(candidate) == 7) {
            ++found;
            if (found == n) {
                return (seq_t)candidate;
            }
        }
        /* advance to the next odd number */
        candidate += (candidate == 2 ? 1 : 2);
    }
}
