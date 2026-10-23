/*
 * A048255  Integers whose sum of divisors is 6^5 = 7776.
 * https://oeis.org/A048255
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = n such that σ(n) = 7776 (sum of divisors of n equals 6⁵)
 * Range:     u8 none, u16 n <= 18, u32 n <= 18, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

/* Target value for the sum of divisors. */
#define TARGET 7776ULL
/* Upper bound for n such that σ(n) = TARGET, since σ(n) ≥ n + 1 for n > 1. */
#define LIMIT 7775UL

/* Store σ for 1 … LIMIT using 64‑bit arithmetic (always safe). */
static uint64_t sigma[LIMIT + 1];

/* Store found terms (maximum known is 18, 25 is comfortably safe). */
static uint64_t terms[25];
static idx_t term_count = 0;
static bool initialized = false;

/* Initialize the sieve for σ and collect terms. */
static void init_sequence(void)
{
    for (idx_t i = 1; i <= LIMIT; ++i) {
        for (idx_t j = i; j <= LIMIT; j += i) {
            sigma[j] += (uint64_t)i;
        }
    }
    for (idx_t n = 1; n <= LIMIT; ++n) {
        if (sigma[n] == TARGET) {
            /* Safe cast because terms fit in 64 bits. */
            terms[term_count++] = (uint64_t)n;
        }
    }
    initialized = true;
}

seq_t a(idx_t n)
{
    if (!initialized) {
        init_sequence();
    }

    if (n == 0) {
        return (seq_t)0;           /* undefined for offset 0 */
    }

    if ((idx_t)(n - 1) < term_count) {
        return (seq_t)terms[n - 1];
    }

    return (seq_t)0;               /* index beyond known terms */
}
