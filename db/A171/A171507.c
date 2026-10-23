/*
 * A171507  a(n) = (5*2^(n+1)-9-(-1)^n)/6-2*n.
 * https://oeis.org/A171507
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5*2^(n+1)-9-(-1)^n)/6-2*n
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS A171507 exactly for all n ≥ 0. */
seq_t a(idx_t n)
{
    /* Compute 2^n / 3 without overflowing seq_t. */
    seq_t pow2_div6;
    if (sizeof(seq_t) <= 8) {
        /* For 64‑bit seq_t use a 128‑bit intermediate to
           safely shift up to n = 63 (the largest n that keeps
           the final value within 64 bits). */
        unsigned __int128 tmp = ((unsigned __int128)1 << n) / 3;
        pow2_div6 = (seq_t)tmp;
    } else {
        /* For larger seq_t types (e.g., __int128 or big integers)
           the shift itself handles arbitrary size. */
        pow2_div6 = ((seq_t)1 << n) / 3;
    }

    /* Start with 5 * (2^n / 3) */
    seq_t term = pow2_div6 * (seq_t)5;

    /* Add 2 for odd n (0 for even n) */
    if (n & 1U)
        term += 2;

    /* Subtract 2*n */
    term -= (seq_t)n * 2U;

    return term;
}
