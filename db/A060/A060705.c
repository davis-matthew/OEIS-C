/*
 * A060705  Singular n X n matrices over GF(3).
 * https://oeis.org/A060705
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = 3^(n^2) – ∏_{k=0}^{n-1} (3^n – 3^k) * * The
 *            implementation uses a fast exponentiation routine for arbitrary
 *            18-64bit * integer types (seq_t) and simple arithmetic that is
 *            valid for both C11 * and C++17. Intermediate values that exceed
 *            the given integer size are * only required for terms...
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A060705 – Singular n×n matrices over GF(3)   */

#include <stdint.h>

/* Fast exponentiation by squaring.  Works for all seq_t and idx_t types. */
static inline seq_t pow_seq_t(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp)
    {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

/*  Singular n×n matrices over GF(3).  */
seq_t a(idx_t n)
{
    /* 3^n */
    seq_t pow3n = pow_seq_t((seq_t)3, n);

    /* 3^(n²)  */
    seq_t total = pow_seq_t((seq_t)3, n * n);

    /* Product of (3^n – 3^k) for k = 0 .. n-1.  */
    seq_t prod   = 1;
    seq_t pow3k  = 1;   /* 3^0 initially */
    for (idx_t k = 0; k < n; ++k)
    {
        seq_t term = pow3n - pow3k;
        prod *= term;
        pow3k *= (seq_t)3;
    }

    return total - prod;
}
