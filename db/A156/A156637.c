/*
 * A156637  Pell numbers A000129 mod 9. Period 24: repeat
 *          0,1,2,5,3,2,7,7,3,4,2,8,0,8,7,4,6,7,2,2,6,5,7,1.
 * https://oeis.org/A156637
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 95, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Pell numbers modulo 9  
   a(0) = 0, a(1) = 1  
   a(n) = (2*a(n-1) + a(n-2)) mod 9   for n ≥ 2
   The sequence repeats with period 24. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    if (n == 1)
        return 1;

    /* iterate the linear recurrence modulo 9 */
    seq_t prev = 0;           /* a(n‑2) */
    seq_t curr = 1;           /* a(n‑1) */
    seq_t next;

    for (idx_t i = 2; i <= n; ++i) {
        next = (2 * curr + prev) % 9;
        prev = curr;
        curr = next;
    }
    return curr;
}
