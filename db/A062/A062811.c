/*
 * A062811  a(n) = Sum_{i=1..n} i^(n - i) + (i - n)^i.
 * https://oeis.org/A062811
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..n} [ i^(n-i) + (i-n)^i ]
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 17, u64 n <= 28, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* fast exponentiation for seq_t values (works for any unsigned integer type or
   arbitrary‑precision integer class that supports operator* and +). */
static inline seq_t pow_seq_t(seq_t base, idx_t exp)
{
    seq_t result = 1;
    seq_t cur     = base;
    while (exp > 0) {
        if (exp & 1U)  result *= cur;
        cur *= cur;
        exp >>= 1U;
    }
    return result;
}

/* OEIS A062811: a(n) = Sum_{i=1..n} i^(n-i) + (i-n)^i */
seq_t a(idx_t n)
{
    seq_t sum_s      = 0;   /* Σ i^(n-i)      (i = 1..n) */
    seq_t sum_even   = 0;   /* Σ (n-i)^i for even i (i < n)  */
    seq_t sum_odd    = 0;   /* Σ (n-i)^i for odd  i (i < n)  */

    for (idx_t i = 1; i <= n; ++i) {
        /* positive part: i^(n-i) */
        seq_t s = pow_seq_t((seq_t)i, n - i);
        sum_s += s;

        /* negative/positive part: (i-n)^i = (-1)^i * (n-i)^i, only for i < n */
        if (i < n) {
            seq_t r = pow_seq_t((seq_t)(n - i), i);
            if ((i & 1U) == 0U) {
                sum_even += r;   /* i even  -> add */
            } else {
                sum_odd  += r;   /* i odd   -> subtract later */
            }
        }
    }

    /* total = Σ i^(n-i) + Σ_even (n-i)^i  */
    seq_t total = sum_s + sum_even;

    /* final result = total - Σ_odd (n-i)^i  */
    if (total < sum_odd) {
        /* This should not happen for legal input – guard against underflow. */
        return 0;
    }
    return total - sum_odd;
}
