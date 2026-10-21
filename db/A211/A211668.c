/*
 * A211668  Number of iterations sqrt(sqrt(sqrt(...(n)...))) such that the
 *          result is < 3.
 * https://oeis.org/A211668
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A211668: Number of iterations of sqrt() until the result is < 3.
   a(n) = the smallest k ≥ 0 such that n < 3^(2^k).  This implementation
   computes k by repeatedly squaring 3 until it exceeds n.  It works for
   any size of seq_t (uint8_t, uint16_t, uint32_t, …, arbitrary‑precision)
   and for any idx_t ≥ 1. */
seq_t a(idx_t n)
{
    if (n < 3)          /* already below 3 */
        return 0;

    seq_t power = 3;    /* current value of 3^(2^k) (k=0)   */
    seq_t ans   = 0;    /* number of iterations performed   */

    /* Iterate while the current power is ≤ n, counting iterations.
       Prevent integer overflow by checking the next multiplication
       against the limit `n`.                                    */
    while ((seq_t)n >= power)
    {
        ans++;    /* we performed another sqrt()  */

        /* If power^2 would exceed n (or overflow), we can stop
           after this iteration since the next power will be > n
           and we will exit the loop.                         */
        if (power > (seq_t)n / power)
            break;

        power = power * power;   /* move to the next 3^(2^(k+1))   */
    }

    return ans;
}
