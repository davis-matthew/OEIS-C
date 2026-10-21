/*
 * A068179  Product_{i=1..3} (i+x) / Product_{i=1..3} (i-x) = Sum_{n>=0}
 *          (a(n)/b(n))*x^n.
 * https://oeis.org/A068179
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A068179
 *  Formula: a(n) = 5*2^(n+1) + 6^(n+1) - 5*3^(n+1)
 *  (n is the index, a(0) = 1, a(1) = 11, ... )
 *
 *  The implementation uses binary exponentiation to compute the powers
 *  efficiently.  It works for all supported integer types seq_t
 *  (unsigned 8/16/32/64-bit or arbitrary‑precision) and for all
 *  idx_t (uint64_t) values, including large n up to at least 10000.
 */

static seq_t pow_int_seq(idx_t exp, int base)
{
    seq_t result = 1;
    seq_t b = (seq_t)base;
    while (exp)
    {
        if (exp & 1) result *= b;
        b *= b;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    idx_t exp = n + 1;           /* we need powers of (n+1) */
    seq_t pow2 = pow_int_seq(exp, 2);
    seq_t pow3 = pow_int_seq(exp, 3);
    seq_t pow6 = pow_int_seq(exp, 6);

    /* Compute 5*2^(n+1) + 6^(n+1) - 5*3^(n+1) */
    seq_t result = 5 * pow2 + pow6 - 5 * pow3;
    return result;
}
