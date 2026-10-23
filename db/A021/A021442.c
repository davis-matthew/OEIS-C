/*
 * A021442  Decimal expansion of 1/438.
 * https://oeis.org/A021442
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(0) = 0; a(n) = the (n‑1)th digit of the repetend 02283105 of
 *            1/438 (for n ≥ 1: a(n) = {0,2,2,8,3,1,0,5}[(n−1) mod 8] )
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The repeating part of 1/438 after the first non‑repeating digit is
       0 2 2 8 3 1 0 5 (length 8).  */
    static const seq_t cycle[8] = {0, 2, 2, 8, 3, 1, 0, 5};

    if (n == 0)
        return 0;
    /* n ≥ 1 guaranteed here, so (n‑1) is non‑negative --------------------------------- */
    return cycle[(n - 1) % 8];
}
