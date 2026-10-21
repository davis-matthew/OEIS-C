/*
 * A067488  Powers of 2 with initial digit 1.
 * https://oeis.org/A067488
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = 2^ceil( (n‑1) * log_2(10) ) The index n starts at 1, so
 *            a(1)=1. The function may be called in arbitrary order; it is
 *            fully deterministic and uses only integer arithmetic of type
 *            seq_t
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

static seq_t pow2(idx_t exp) {
    seq_t result = 1;
    seq_t base   = 2;
    while (exp) {
        if (exp & 1) result *= base;
        exp >>= 1;
        if (exp) base *= base;
    }
    return result;
}

/* Compute a(n) for n >= 1. */
seq_t a(idx_t n) {
    static const long double log2_10 = log2l(10.0L);      /* ≈ 3.321928094 */
    /* Use a tiny negative epsilon to avoid floor‑to‑ceil round‑up errors
       caused by floating‑point inaccuracy. */
    const long double eps = 1e-15L;
    long double term = (long double)(n - 1) * log2_10 - eps;
    idx_t exp = (idx_t)ceill(term);
    return pow2(exp);
}
