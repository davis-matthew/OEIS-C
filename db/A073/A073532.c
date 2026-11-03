/*
 * A073532  Number of n-digit primes with all digits distinct.
 * https://oeis.org/A073532
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Validated: partial, pass, 87 terms through n = 87 (2026-11-01)
 * Helpers:   is_prime_u32, mulmod_u32, powmod_u32
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
#include "../utils/is_prime_u32.h"
#include "../utils/mulmod_u32.h"
#include "../utils/powmod_u32.h"

/* Recursively generate all distinct‑digit numbers of a given length. */
static void gen_distinct_digits(unsigned len, unsigned pos,
                                uint16_t used, uint32_t num,
                                seq_t *cnt)
{
    if (pos == len) {
        if (is_prime_u32(num)) cnt[len - 1]++;   /* length is len */
        return;
    }
    for (unsigned d = 0; d < 10; ++d) {
        if (pos == 0 && d == 0) continue;     /* no leading zero */
        if (used & (1U << d)) continue;        /* digit already used */
        gen_distinct_digits(len, pos + 1,
                            used | (1U << d),
                            num * 10 + d, cnt);
    }
}

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
