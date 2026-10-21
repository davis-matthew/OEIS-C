/*
 * A212981  Number of (w,x,y) with all terms in {0,...,n} and w <= x + y and x
 *          < y.
 * https://oeis.org/A212981
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 3*a(n-1)-2*a(n-2)-2*a(n-3)+3*a(n-4)-a(n-5),
   with a(0)=0, a(1)=2, a(2)=8, a(3)=21, a(4)=43. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 2;
    if (n == 2) return 8;
    if (n == 3) return 21;
    if (n == 4) return 43;

    seq_t a0 = 0, a1 = 2, a2 = 8, a3 = 21, a4 = 43, next;
    for (idx_t i = 5; i <= n; ++i) {
        next = 3*a4 - 2*a3 - 2*a2 + 3*a1 - a0;
        a0 = a1; a1 = a2; a2 = a3; a3 = a4; a4 = next;
    }
    return a4;
}
