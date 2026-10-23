/*
 * A016169  a(n) = 7^n - 6^n.
 * https://oeis.org/A016169
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7^n - 6^n (Sequence A016169: a(n) = 7^n - 6^n, a(0)=0,
 *            a(1)=1, …)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow_uint64(seq_t base, idx_t exp)
{
    /* Fast exponentiation by squaring for unsigned integer types
       and possibly for arbitrary‑precision seq_t.  The exponent exp
       is an idx_t (uint64_t).  The multiplication stays within the
       bounds of seq_t, because the caller uses the result only
       when it fits. */
    seq_t result = 1;
    seq_t b = base;
    while (exp > 0) {
        if (exp & 1ULL)
            result *= b;
        if (exp > 1)
            b *= b;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Compute 7^n and 6^n and return their difference. */
    seq_t p7 = pow_uint64(7, n);
    seq_t p6 = pow_uint64(6, n);
    return p7 - p6;
}
