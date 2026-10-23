/*
 * A175408  a(n) + a(n - 1) is alternatively a cube or a square.
 * https://oeis.org/A175408
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A175408
    a(1)=1,
    a(2n)   = (least cube > a(2n-1)) - a(2n-1),
    a(2n+1) = (least square > a(2n)) - a(2n).
    The sequence is periodic of length 10:
    {7,2,6,3,5,4,4,5,3,1}.  */

seq_t a(idx_t n)
{
    /* Periodic table of values for indices 1..10 */
    static const seq_t period[10] = {
        1, 7, 2, 6, 3, 5, 4, 4, 5, 3
    };

    /* For safety, return 0 if n==0 (though n≥1 in the sequence) */
    if (n == 0) return 0;

    idx_t i = (n - 1) % 10;      /* index into the period table */
    return period[i];
}
