/*
 * A217520  Base-3 state complexity of partitioned deterministic finite
 *          automaton (PDFA) for the periodic sequence (123...n)*.
 * https://oeis.org/A217520
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * ord_{n/3^ν}(3) + (3^ν - 1)/2, where ν is the exponent
 *            of 3 in n and ord_m(3) is the multiplicative order of 3 modulo m
 *            (defined for m coprime to 3; for m = 1 the order is taken to be
 *            1)
 * Range:     u8 n <= 16, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Validated: standard, pass, 69 terms through n = 94 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute the multiplicative order of 3 modulo m (m must be coprime to 3).
   For m == 1 the order is defined as 1. */
static idx_t multiplicative_order_3(idx_t m)
{
    if (m == 1) return 1;
    idx_t t = 1;
    idx_t cur = 3 % m;
    /* Iterate until cur == 1.  We check from t=1, cur already is 3 mod m. */
    while (cur != 1) {
        /* Use 128–bit arithmetic to avoid overflow of cur*3. */
        cur = (unsigned __int128)cur * 3 % m;
        ++t;
    }
    return t;
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;   /* behaviour below the offset does not matter */

    /* Split n into 3^ν * m with m not divisible by 3. */
    idx_t temp = n;
    seq_t pow3 = 1;            /* 3^ν */
    idx_t m = n;
    while (m % 3 == 0) {
        m /= 3;
        pow3 = pow3 * 3;
    }

    /* Compute the multiplicative order of 3 modulo m. */
    idx_t order = multiplicative_order_3(m);

    /* Compute the additive part (3^ν - 1)/2. */
    seq_t add = (pow3 - 1) / 2;

    /* Result: n * order + add. */
    return ((seq_t)n) * (seq_t)order + add;
}
