/*
 * A123108  a(n) = a(n-1) + a(n-2) - a(n-3), for n > 3, with a(0)=1, a(1)=0,
 *          a(2)=1, a(3)=1.
 * https://oeis.org/A123108
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n - 1 + (-1)^n)/4 for n > 0; a(0) = 1
 * Range:     u8 n <= 184, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1) + a(n-2) - a(n-3) with a(0)=1, a(1)=0, a(2)=1, a(3)=1 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;

    seq_t val = (seq_t)n;
    seq_t temp;

    /* For n >= 1, (2*n - 1 + (-1)^n) simplifies to:
       - even n: 2*n
       - odd  n: 2*n - 2
    */
    if ((n & 1) == 0) {
        temp = 2 * val;
    } else {
        temp = 2 * val - (seq_t)2;
    }

    return temp / (seq_t)4;
}
