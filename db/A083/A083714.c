/*
 * A083714  (greatest prime <= n) - (greatest prime factor of n).
 * https://oeis.org/A083714
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   largest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A083714:  a(n) = (greatest prime ≤ n) – (greatest prime factor of n)
    n ≥ 1, offset 1 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/largest_prime_factor.h"

/* ------------------------------------------------------------------ */
/*  Helper:  test whether a 64‑bit integer is prime (trial division). */
/*  Works for all n used in the tests (≤ 10 000).                        */
static bool is_prime(idx_t n)
{
    if (n < 2U)          return false;
    if ((n & 1U) == 0U)  return n == 2U;
    for (idx_t i = 3U; i * i <= n; i += 2U)
        if (n % i == 0U) return false;
    return true;
}

/* ------------------------------------------------------------------ */
/*  Helper:  largest prime divisor of a 64‑bit integer (trial division). */

/* ------------------------------------------------------------------ */
/*  Main sequence function                                            */
seq_t a(idx_t n)
{
    /* Handle offset n = 1 */
    if (n < 2U) return (seq_t)0U;

    /*  Greatest prime ≤ n */
    idx_t p = n;
    while (!is_prime(p)) --p;

    /*  Greatest prime factor of n */
    uint64_t q = largest_prime_factor((uint64_t)n);

    /*  Return the difference, cast to seq_t */
    return (seq_t)p - (seq_t)q;
}
