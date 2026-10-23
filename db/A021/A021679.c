/*
 * A021679  Decimal expansion of 1/675.
 * https://oeis.org/A021679
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(0)=0, a(1)=0; for n ≥ 2, a(n)=c[(n-2) mod 3] where c={1,4,8}
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* return 0 for the two leading terms */
    if (n < (idx_t)2)
        return (seq_t)0;

    /* cycle through 1, 4, 8 */
    static const seq_t cycle[3] = {(seq_t)1, (seq_t)4, (seq_t)8};

    /* compute (n-2) mod 3 safely; idx_t is unsigned, so no underflow here */
    idx_t idx = n - (idx_t)2;
    idx_t r = idx % (idx_t)3;   /* r ∈ {0,1,2} */

    return cycle[r];
}
