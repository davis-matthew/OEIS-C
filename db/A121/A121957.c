/*
 * A121957  Expansion of x^2*(43 -11*x -148*x^2 +23*x^3)/(1 -2*x -7*x^2 +7*x^3
 *          +12*x^4 -2*x^5).
 * https://oeis.org/A121957
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 18, u64 n <= 37, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A121957:
 * a(n) = 2*a(n-1) + 7*a(n-2) - 7*a(n-3) - 12*a(n-4) + 2*a(n-5)
 * Initial values (offset 1):
 *   a(1)=0, a(2)=43, a(3)=75, a(4)=303, a(5)=853
 * This implementation uses a simple iterative approach with a circular
 * buffer of the last five terms, avoiding dynamic memory allocation.
 */

#include <stdint.h> /* for uintptr_t if desired, not required */

seq_t a(idx_t n)
{
    /* Handle the first five terms directly. */
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)43;
    if (n == 3) return (seq_t)75;
    if (n == 4) return (seq_t)303;
    if (n == 5) return (seq_t)853;

    /* Circular buffer: a_nm5..a_nm1 hold a(n-5)..a(n-1) respectively. */
    seq_t a_nm5 = (seq_t)0;   /* a(1)   */
    seq_t a_nm4 = (seq_t)43;  /* a(2)   */
    seq_t a_nm3 = (seq_t)75;  /* a(3)   */
    seq_t a_nm2 = (seq_t)303; /* a(4)   */
    seq_t a_nm1 = (seq_t)853; /* a(5)   */

    seq_t a_n;  /* current term */

    for (idx_t i = 6; i <= n; ++i)
    {
        a_n = 2 * a_nm1 + 7 * a_nm2 - 7 * a_nm3 - 12 * a_nm4 + 2 * a_nm5;

        /* shift the window: the term a(i-4) becomes a_nm5, etc. */
        a_nm5 = a_nm4;
        a_nm4 = a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_nm1;
}
