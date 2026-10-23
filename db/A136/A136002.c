/*
 * A136002  Numbers that are not the sum, minus 1, of a Pythagorean triple.
 * https://oeis.org/A136002
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth positive integer that is NOT of the form a + b +
 *            c – 1 for any Pythagorean triple (a, b, c)
 * Range:     u8 n <= 213, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd_uint64.h"

/* Upper bound for perimeters (n+1) that we pre‑compute. */
#define MAX_PERI      300000   /* large enough to cover n ≤ 10000   */
#define MAX_OUTPUT    20000    /* maximum terms stored in the cache  */

/* Global tables used only inside the function a(). */
static bool peri[MAX_PERI + 2];   /* peri[p] == 1 if p is a triple perimeter */
static bool peri_ready = false;

/* Greatest common divisor for uint64_t. */

/* Compute all perimeters a+b+c for primitive and non‑primitive
   Pythagorean triples up to MAX_PERI+1. */
static void compute_peri(void)
{
    if (peri_ready) return;

    for (uint64_t m = 2;; ++m)
    {
        bool stop_outer = false;
        for (uint64_t n = 1; n < m; ++n)
        {
            if (((m - n) & 1) == 0) continue;          /* same parity => not primitive */
            if (gcd_uint64(m, n) != 1) continue;       /* not coprime => not primitive */

            uint64_t p = 2 * m * (m + n);              /* perimeter a + b + c */
            if (p > (uint64_t)MAX_PERI + 1)
            {
                if (n == 1) stop_outer = true;        /* outer loop can stop */
                break;
            }

            for (uint64_t k = 1; k * p <= (uint64_t)MAX_PERI + 1; ++k)
                peri[k * p] = true;
        }
        if (stop_outer) break;
    }
    peri_ready = true;
}

/* Cache for the sequence terms. */
static seq_t seq_cache[MAX_OUTPUT];
static idx_t seq_len = 0;

/* Pre‑compute the sequence up to MAX_OUTPUT terms. */
static void precompute_sequence(void)
{
    if (seq_len > 0) return;

    compute_peri();

    for (uint64_t x = 1; x <= (uint64_t)MAX_PERI; ++x)
    {
        if (!peri[x + 1])          /* x is NOT of the form p-1 */
        {
            if (seq_len < MAX_OUTPUT)
                seq_cache[seq_len++] = (seq_t)x;
        }
    }
}

/* Public function required by the build system. */
seq_t a(idx_t n)
{
    precompute_sequence();
    return seq_cache[n - 1];   /* n is 1‑based */
}
