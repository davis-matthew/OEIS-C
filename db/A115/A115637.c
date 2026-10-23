/*
 * A115637  In the binary expansion of n+2, transform 0->1 and 1->0 then
 *          interpret as base 4.
 * https://oeis.org/A115637
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, look
 * Formula:   a(n) = Σ_{i≥0} (1 - bit_i(n+2)) * 4^i
 * Range:     u8 n <= 29, u16 n <= 509, u32 n <= 9203, u64 n <= 9203,
 *            u128 n <= 9203, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*           where bit_i(x) is the i‑th bit (0‑based) of x in binary. */

seq_t a(idx_t n)
{
    /* Convert n to seq_t early to avoid overflow on idx_t addition. */
    seq_t m = (seq_t)n + (seq_t)2;     /* m = n + 2  (value in seq_t) */
    seq_t result = 0;
    seq_t pow4   = 1;                  /* 4^0 initially */

    /* Process all bits of m from LSB to the most significant bit. */
    for (idx_t i = 0; (m >> i) != 0; ++i)
    {
        /* If bit i of m is 0, add 4^i to the result. */
        if (((m >> i) & (seq_t)1) == 0)
            result += pow4;
        /* Prepare 4^(i+1) for the next iteration. */
        pow4 *= (seq_t)4;
    }
    return result;
}
