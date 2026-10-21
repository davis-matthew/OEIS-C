/*
 * A083537  a(0) = a(1) = 0, a(2n) = a(n)+1, a(2n+1) = a(n-1).
 * https://oeis.org/A083537
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 92, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A083537
   a(0) = a(1) = 0
   a(2n)   = a(n) + 1
   a(2n+1) = a(n-1)   (for n ≥ 1)
*/
seq_t a(idx_t n)
{
    /* Use an iterative approach that follows the recurrence,
       avoiding recursion and memoisation. */
    seq_t res = 0;          /* result accumulator */

    /* Process until the value is reduced to 0 or 1 (the base cases). */
    while (n > 1)
    {
        if ((n & 1) == 0)   /* n is even: a(n) = a(n/2) + 1 */
        {
            n >>= 1;
            res += 1;
        }
        else                /* n is odd: a(n) = a((n-3)/2) */
        {
            n = (n - 3) >> 1;
            /* no change to res */
        }
    }
    return res;
}
