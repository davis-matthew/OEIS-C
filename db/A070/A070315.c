/*
 * A070315  Third diagonal of triangle in A046739.
 * https://oeis.org/A070315
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^n - (3*n+2)*2^(n-1) + 2*n^2
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for any n ≥ 4.  The function works for both
 * 64‑bit and arbitrary‑precision builds: arithmetic is performed
 * on the type seq_t, which must support +, -, *, and cast from
 * integer constants.  No storage of intermediate big powers is
 * required, so the implementation is safe in C and in C++17. */
seq_t a(idx_t n)
{
    /* 3^n */
    seq_t pow3 = (seq_t)1;
    for (idx_t i = 0; i < n; ++i) {
        pow3 *= (seq_t)3;
    }

    /* 2^(n-1) */
    seq_t pow2 = (seq_t)1;
    for (idx_t i = 1; i < n; ++i) {
        pow2 *= (seq_t)2;
    }

    /* (3*n+2)*2^(n-1) */
    seq_t coeff = (seq_t)( (seq_t)3 * n + (idx_t)2 );
    seq_t term2 = coeff * pow2;

    /* 2*n^2 */
    seq_t nsq   = (seq_t)n * (seq_t)n;
    seq_t term3 = nsq * (seq_t)2;

    return pow3 - term2 + term3;
}
