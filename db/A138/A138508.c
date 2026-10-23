/*
 * A138508  Semiprime analog of Riesel problem: start with n; repeatedly
 *          double and add 1 until reach a semiprime. Sequence gives number of
 *          steps to reach a semiprime or 0 if no semiprime is ever reached.
 * https://oeis.org/A138508
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = smallest m ≥ 1 such that (n+1)·2^m − 1 is a semiprime;
 *            returns 0 if no such m is found
 * Range:     u8 n <= 14, u16 n <= 111, u32 n <= 111, u64 n <= 111,
 *            u128 n <= 111, big any n
 * Helpers:   count_prime_factors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/count_prime_factors.h"

static bool is_semiprime(seq_t x) {
    return count_prime_factors(x) == 2;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;                 /* offset starts at 1, but guard */
    const seq_t base = (seq_t)(n + 1);

    /* m = 1: (n+1)*2^1 - 1 */
    seq_t val = base * 2;
    val -= 1;
    idx_t m   = 1;
    /* Safe upper bound: we will stop if value overflows or when we reach 2000 steps. */
    for ( ; m <= 2000; ++m ) {
        if (is_semiprime(val)) return (seq_t)m;
        seq_t next = val * 2 + 1;          /* (n+1)*2^(m+1) - 1 = 2*val + 1 */
        /* Detect overflow for fixed-width unsigned seq_t (e.g., 64-bit). */
        if (next <= val) break;
        val = next;
    }
    return (seq_t)0;  /* no semiprime found within bound */
}
