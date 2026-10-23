/*
 * A116917  a(1)=a(2)=1. a(n) = A006530(a(n-1)) + A006530(a(n-2)).
 * https://oeis.org/A116917
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 94, u16 n <= 94, u32 n <= 94, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A116917: a(1)=1,a(2)=1,a(3)=2,a(4)=3,a(5)=5;
   For n>=6 the sequence repeats 8,7,9,10 with period 4
   (i.e., a(4k+2)=8, a(4k+3)=7, a(4k+4)=9, a(4k+5)=10). */
seq_t a(idx_t n)
{
    static const seq_t init[6] = {0, 1, 1, 2, 3, 5};
    if (n < 6) return init[n];
    idx_t r = n % 4;
    if (r == 2) return 8; /* a(4k+2) */
    if (r == 3) return 7; /* a(4k+3) */
    if (r == 0) return 9; /* a(4k+4) */
    return 10;            /* r == 1, a(4k+5) */
}
