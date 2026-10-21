/*
 * A052100  a(n) = lcm(n, phi(n), n - phi(n)).
 * https://oeis.org/A052100
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lcm(n, φ(n), n - φ(n))
 * Range:     u8 n <= 14, u16 n <= 68, u32 n <= 2594, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   phi_uint64, seq_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/seq_gcd.h"
#include "../utils/phi_uint64.h"

/* Helper: lcm for seq_t values, returns 0 if either argument is 0 */
static inline seq_t lcm_seq(seq_t a, seq_t b)
{
    if (a == 0 || b == 0)
        return (seq_t)0;
    seq_t g = seq_gcd(a, b);
    return (a / g) * b;
}

/* Sequence function */
seq_t a(idx_t n)
{
    seq_t n_val   = (seq_t)n;                           /* n as seq_t */
    seq_t phi_val = (seq_t)phi_uint64((uint64_t)n);     /* φ(n) */
    seq_t cot_val = n_val - phi_val;                    /* n - φ(n) */

    seq_t l1 = lcm_seq(n_val, phi_val);
    return lcm_seq(l1, cot_val);
}
