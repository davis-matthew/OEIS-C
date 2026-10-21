/*
 * A073532  Number of n-digit primes with all digits distinct.
 * https://oeis.org/A073532
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   gen_distinct_digits, is_prime_u32, mulmod32, powmod32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: enumerate all distinct‑digit numbers with 1–9 digits
   and count how many of them are prime.
   For n ≥ 10 the answer is 0 because no 10‑digit prime has all digits
   distinct. This method pre‑computes the results once and then
   returns the cached value. */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/gen_distinct_digits.h"
#include "../utils/is_prime_u32.h"
#include "../utils/mulmod32.h"
#include "../utils/powmod32.h"

/* Pre‑compute the counts for lengths 1…9. */
static void precompute(seq_t *cnt)
{
    for (unsigned len = 1; len <= 9; ++len) {
        cnt[len - 1] = (seq_t)0;
        gen_distinct_digits(len, 0, 0, 0, cnt);
    }
}

/* Cached results: indices 0..8 correspond to n = 1..9. */
static seq_t cache[10] = { 0 };
static bool computed = false;

seq_t a(idx_t n)
{
    if (n < 1ULL) return 0;          /* offset 1: behaviour below is irrelevant */
    if (n >= 10ULL) return 0;        /* no 10‑digit distinct‑digit primes */
    if (!computed) {
        precompute(cache);
        computed = true;
    }
    return cache[(size_t)(n - 1)];
}
