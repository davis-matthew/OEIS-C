/*
 * A058035  Largest 4th-power-free number dividing n.
 * https://oeis.org/A058035
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, mult
 * Formula:   not stated
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A058035: Largest 4th‑power‑free number dividing n
 *   Formula: a(n) = ∏_{p^e‖n} p ^ min(e,3)
 *   i.e. for each prime divisor p of n, use the factor p^k with k = min(e,3)
 *   where e is the exponent of p in the prime factorisation of n.
 *
 *  The algorithm factors n by trial division, taking into account that only
 *  exponents up to 3 influence the result.  The function is purely
 *  functional: it is independent of previous calls and works for any
 *  n ≥ 1.
 *
 *  Time complexity: O(√n / log n) for 64‑bit n (practical for n up to ~10⁹
 *  in the required performance window).  The function never uses
 *  floating‑point arithmetic or external helper libraries. */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Decompose n and multiply prime powers up to exponent 3. */
    uint64_t m = (uint64_t)n;
    seq_t result = (seq_t)1;

    /* Handle factor 2 */
    if (m % 2ULL == 0) {
        int exp = 0;
        do {
            m /= 2ULL;
            exp++;
        } while (m % 2ULL == 0);
        int use = exp > 3 ? 3 : exp;
        seq_t mult = (seq_t)1;
        for (int i = 0; i < use; ++i) mult *= (seq_t)2;
        result *= mult;
    }

    /* Handle odd factors */
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p != 0) continue;
        int exp = 0;
        do {
            m /= p;
            exp++;
        } while (m % p == 0);
        int use = exp > 3 ? 3 : exp;
        seq_t mult = (seq_t)1;
        for (int i = 0; i < use; ++i) mult *= (seq_t)p;
        result *= mult;
    }

    /* Remaining prime factor (if any) */
    if (m > 1) {
        result *= (seq_t)m;   /* exponent 1 always ≤ 3 */
    }

    return result;
}
