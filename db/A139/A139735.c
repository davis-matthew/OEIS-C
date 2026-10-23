/*
 * A139735  a(n) = 10^n mod 4^n.
 * https://oeis.org/A139735
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10^n mod 4^n
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*            = 2^n * (5^n mod 2^n) */

/* Computes a(n) for any n >= 0.  The routine works for
 * arbitrary‑precision seq_t as well as for native
 * unsigned integer types.  For very large n (when 2^n cannot be
 * represented in seq_t) the function may return an overflowed
 * value, which is acceptable because only terms that fit in
 * seq_t are required to be correct. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* 10^0 mod 1 = 0 */

    /* Compute 2^n in a type‑safe manner by shifting left one bit
     * at a time.  This avoids undefined behaviour when n is
     * larger than the number of bits in seq_t. */
    seq_t two_n = (seq_t)1;
    for (idx_t i = 0; i < n; ++i)
        two_n <<= 1;               /* two_n = 2^n */

    seq_t mask = two_n - (seq_t)1; /* mask = 2^n – 1 */

    /* Binary exponentiation of 5^n modulo 2^n.
     * All intermediate products are masked to keep only the
     * lower n bits. */
    seq_t r = (seq_t)1;            /* r = 5^0 mod 2^n */
    seq_t base = (seq_t)5;         /* base = 5 */
    idx_t e = n;                   /* exponent remaining */

    while (e > 0) {
        if (e & (idx_t)1)
            r = (r * base) & mask;
        base = (base * base) & mask;
        e >>= 1;
    }

    /* a(n) = 2^n * r  (no overflow logic needed for the part that fits) */
    return two_n * r;
}
