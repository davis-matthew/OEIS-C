/*
 * A071720  Number of spanning trees in K_{n}-e, the complete graph on n nodes
 *          minus an edge (n > 1).
 * https://oeis.org/A071720
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n - 2) * n^(n - 3)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n <= 2 the sequence is defined to be 0 */
    if (n <= 2)
        return 0;

    /* Compute n^(n-3) by exponentiation by squaring */
    seq_t base   = (seq_t)n;          /* the base n      */
    idx_t  exp   = n - 3;             /* exponent n-3    */
    seq_t power  = 1;                 /* result accumulator */

    while (exp > 0)
    {
        if (exp & 1)
            power = power * base;     /* multiply by current base if exponent bit is set */
        base  = base * base;          /* square the base for the next bit */
        exp >>= 1;
    }

    /* Multiply by the coefficient (n-2) */
    power = power * (seq_t)(n - 2);

    return power;
}
