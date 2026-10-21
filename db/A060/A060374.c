/*
 * A060374  a(n)=p+q, where n=p-q and p, q, p+q are in A005836 (integers
 *          written without 2 in base 3).
 * https://oeis.org/A060374
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 121, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A060374
 *
 * Definition:
 *   a(n) = p + q where n = p – q and p, q, p + q are in A005836
 *   (integers whose base‑3 expansion contains only digits 0 or 1).
 *
 * Observation:
 *   Write n in balanced ternary (digits –1, 0, 1).  
 *   Let the balanced digits be d_i.  Then
 *     p = Σ_{d_i=1} 3^i
 *     q = Σ_{d_i=-1} 3^i
 *     a(n) = p + q = Σ_{d_i≠0} 3^i
 *   Thus a(n) is obtained by converting each non‑zero digit of the
 *   balanced‑ternary representation of n into a 1 and summing the
 *   corresponding powers of 3.
 *
 * Algorithm:  Convert n to balanced ternary iteratively.
 *   At each step we compute r = n % 3.
 *   If r = 2 we treat it as digit –1, add 3^i to the answer,
 *   and carry 1 to the next higher position (n = n/3 + 1).
 *   If r = 1 treat it as digit +1, add 3^i to the answer.
 *   If r = 0 do nothing.
 *   Multiply the power (3^i) by 3 for the next iteration.
 *
 * Complexity:  O(log₃(n)) time, O(1) memory. */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t ans   = 0;   /* result */
    seq_t power = 1;   /* current 3^i */

    while (n > 0)
    {
        idx_t r = n % 3;
        if (r != 0)
        {
            ans += power;
            if (r == 2)
                n = n / 3 + 1;   /* treat 2 as –1 with carry */
            else
                n = n / 3;       /* r == 1 */
        }
        else
        {
            n = n / 3;
        }
        power *= 3;
    }
    return ans;
}
