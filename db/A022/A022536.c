/*
 * A022536  Nexus numbers (n+1)^20 - n^20.
 * https://oeis.org/A022536
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 2, u64 n <= 8, u128 n <= 90,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = (n+1)^20 - n^20
 * Implemented via the binomial expansion:
 *   a(n) = Σ_{k=0}^{19} binom(20,k) * n^k
 */

#include "../utils/binom.h"

/* Multiply base by itself exp times.
 * Works for both built‑in and arbitrary‑precision seq_t. */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    for (idx_t i = 0; i < exp; ++i)
        result *= base;
    return result;
}

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    seq_t result = 0;

    for (idx_t k = 0; k <= 19; ++k) {
        seq_t coef = binom(20, k);          /* binomial coefficient (20,k) */
        seq_t power = pow_seq(n_seq, k);    /* n^k */
        result += coef * power;             /* accumulate term */
    }
    return result;
}
