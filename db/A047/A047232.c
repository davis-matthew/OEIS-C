/*
 * A047232  Smallest k > n where n is a 4th power modulo k.
 * https://oeis.org/A047232
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = smallest k > n such that n is a 4th‑power residue modulo
 *            k (i.e. ∃x: x⁴ ≡ n (mod k))
 * Range:     u8 n <= 238, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 87 terms through n = 9857 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Test whether n is a fourth‑power residue modulo k.
   The routine factorises k into prime powers and checks each prime power
   separately.  For each prime power pᵉ dividing k we test whether
   ∃x : x⁴ ≡ n (mod pᵉ) by brute‑force enumeration of residues mod pᵉ.
   If all prime powers admit a solution then a solution modulo k exists
   by the Chinese Remainder Theorem. */
static bool is_fourth_power_residue(idx_t n, idx_t k)
{
    idx_t m = k;                   /* remaining factor to process */

    for (idx_t p = 2; p * p <= m; ++p) {
        if (m % p != 0)
            continue;

        /* Compute the prime power pᵉ and its modulus pe */
        idx_t pe = 1;
        while (m % p == 0) {
            m /= p;
            pe *= p;
        }

        /* Residue of n modulo pe */
        seq_t r = (seq_t)n % pe;

        /* Brute‑force search for a fourth power congruent to r (mod pe) */
        bool found = false;
        for (seq_t x = 0; x < pe; ++x) {
            seq_t t = (x * x) % pe;      /* x²  (mod pe)   */
            t = (t * t) % pe;            /* x⁴  (mod pe)   */
            if (t == r) {
                found = true;
                break;
            }
        }
        if (!found)
            return false;
    }

    /* Remaining prime factor (if any) is m itself, a prime ≤ k. */
    if (m > 1) {
        idx_t pe = m;         /* m = p¹ */
        seq_t r = (seq_t)n % pe;

        bool found = false;
        for (seq_t x = 0; x < pe; ++x) {
            seq_t t = (x * x) % pe;
            t = (t * t) % pe;
            if (t == r) {
                found = true;
                break;
            }
        }
        if (!found)
            return false;
    }

    return true;
}

seq_t a(idx_t n)
{
    /* The sequence is defined for all n ≥ 0.  For n = 0 the answer is 1. */
    if (n == 0)
        return 1;

    for (idx_t k = n + 1; ; ++k) {
        if (is_fourth_power_residue(n, k))
            return (seq_t)k;
    }
}
