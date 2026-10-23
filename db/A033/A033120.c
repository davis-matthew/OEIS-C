/*
 * A033120  Base-2 digits of a(n) are, in order, the first n terms of the
 *          periodic sequence with initial period 1,0,1.
 * https://oeis.org/A033120
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = floor(5*2^n / 7)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence (no overflow on 64‑bit builds for n ≤ 64):
   a(1) = 1,  rem = 5*2^1 mod 7 = 10 mod 7 = 3
   For i from 2 to n:
       a_i = 2*a_{i-1} + (2*rem)/7
       rem  = (2*rem) % 7
*/
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* unused, offset starts at 1 */
    seq_t q = 1;                           /* a(1) */
    unsigned int rem = 3;                  /* 10 mod 7 */
    if (n == 1) return q;
    for (idx_t i = 2; i <= n; ++i) {
        unsigned int tr = rem << 1;        /* 2*rem */
        q = q + q + (tr / 7);              /* q = 2*q + floor(2*rem/7) */
        rem = tr % 7;                      /* new remainder */
    }
    return q;
}
