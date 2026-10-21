/*
 * A094537  A094536/2.
 * https://oeis.org/A094537
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   * b(0) = 1 * b(n) = 2*b(n-1) if n is odd * 2*b(n-1) - b(n/2) if
 *            n is even * a(0) = 0 * a(n) = 2^(n-1) - b(n)/2 (n ≥ 1) * * The
 *            routine keeps the values of b(n) and the powers of two * in
 *            static arrays so that arbitrary order of calls is handled *
 *            efficiently. The arrays are large enough for n up...
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* Limit for preallocated storage (larger than needed for n ≤ 10000). */
    enum { MAX_N = 20000 };

    /* Static working arrays and state.  Initialized on the first call. */
    static int          initialized = 0;
    static idx_t        max_computed = 0;
    static seq_t        b[MAX_N + 1];
    static seq_t        pow2[MAX_N + 1];

    /* Basic guard for the out‑of‑range request. */
    if (n > MAX_N) {
        /* In practice this should never happen for the intended limits. */
        return (seq_t)0;
    }

    if (!initialized) {
        /* First initialisation: base cases. */
        b[0]      = (seq_t)1;
        pow2[0]   = (seq_t)1;        /* 2^0 */
        max_computed = 0;
        initialized = 1;
    }

    if (n == 0) {
        /* a(0) is defined as 0. */
        return (seq_t)0;
    }

    /* Grow the tables up to the requested index n. */
    if (n > max_computed) {
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            if ((i & 1u) != 0) {
                /* n odd  -> b(n) = 2*b(n-1) */
                b[i] = (seq_t)2 * b[i - 1];
            } else {
                /* n even -> b(n) = 2*b(n-1) - b(n/2) */
                b[i] = (seq_t)2 * b[i - 1] - b[i / 2];
            }

            /* pow2[i] = 2^i */
            pow2[i] = pow2[i - 1] * (seq_t)2;
        }
        max_computed = n;
    }

    /* a(n) = 2^(n-1) - b(n)/2  (n ≥ 1) */
    seq_t half_b = b[n] / (seq_t)2;          /* exact division by two */
    seq_t result = pow2[n - 1] - half_b;
    return result;
}
