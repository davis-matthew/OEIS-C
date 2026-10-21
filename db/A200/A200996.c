/*
 * A200996  Least upper limit for numbers u and v such that u*n-1 and v*n+1
 *          are both prime, u and v not necessarily distinct.
 * https://oeis.org/A200996
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 2298, u32 n <= 8943, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = max { minimal u : u·n‑1 is prime,
 *                      minimal v : v·n+1 is prime }.
 * The function is fully generic for all supported seq_t / idx_t types. */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for indices < 1 */

    seq_t u_val = 0;
    seq_t v_val = 0;

    /* Find smallest u with u*n‑1 prime */
    for (idx_t i = 1; ; ++i) {
        seq_t val = (seq_t)i * (seq_t)n - (seq_t)1;
        if (is_prime_big(val)) { u_val = (seq_t)i; break; }
    }

    /* Find smallest v with v*n+1 prime */
    for (idx_t j = 1; ; ++j) {
        seq_t val = (seq_t)j * (seq_t)n + (seq_t)1;
        if (is_prime_big(val)) { v_val = (seq_t)j; break; }
    }

    return (u_val > v_val) ? u_val : v_val;
}
