/*
 * A016127  Expansion of g.f. 1/((1-2*x)*(1-5*x)).
 * https://oeis.org/A016127
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = Σ_{i=0..n} 5^i * 2^{n-i}
 *
 * This implementation evaluates the sum iteratively, preserving
 * values that fit into seq_t.  No intermediate value overflows
 * seq_t for indices where a(n) itself fits into seq_t, and the
 * function works in O(n) time for all n ≥ 0.
 */

seq_t a(idx_t n)
{
    seq_t pow5 = 1;   /* 5^i  */
    seq_t pow2 = 1;   /* 2^{n-i}  */
    seq_t sum  = 0;

    /* initialise pow2 to 2^n */
    for (idx_t i = 0; i < n; ++i)
        pow2 *= 2;

    /* iterate over i = 0 .. n */
    for (idx_t i = 0; i <= n; ++i) {
        sum += pow5 * pow2;  /* term 5^i * 2^{n-i} */
        pow5 *= 5;          /* next power of 5     */
        if (i < n)          /* next power of 2 downwards */
            pow2 /= 2;
    }

    return sum;
}
