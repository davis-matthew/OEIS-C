/*
 * A059169  Number of partitions of n into 3 parts which form the sides of a
 *          nondegenerate isosceles triangle.
 * https://oeis.org/A059169
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=0, a(2)=0, a(3)=1, a(4)=0, a(5)=1; recurrence:
 *            a(n)=a(n-1)+a(n-4)-a(n-5) for n≥6
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the n-th term of OEIS A059169. */
seq_t a(idx_t n)
{
    /* small n: return the initial values directly */
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)0;
    if (n == 3) return (seq_t)1;
    if (n == 4) return (seq_t)0;
    if (n == 5) return (seq_t)1;

    /* variables holding the last five computed values:
       a(n-5), a(n-4), a(n-3), a(n-2), a(n-1)  */
    seq_t a_n5 = (seq_t)0;  /* a(1)  */
    seq_t a_n4 = (seq_t)0;  /* a(2)  */
    seq_t a_n3 = (seq_t)1;  /* a(3)  */
    seq_t a_n2 = (seq_t)0;  /* a(4)  */
    seq_t a_n1 = (seq_t)1;  /* a(5)  */

    seq_t a_n;              /* a(n) */

    for (idx_t i = 6; i <= n; ++i) {
        a_n = a_n1 + a_n4 - a_n5;   /* a(n)=a(n-1)+a(n-4)-a(n-5) */

        /* shift the window for the next iteration */
        a_n5 = a_n4;
        a_n4 = a_n3;
        a_n3 = a_n2;
        a_n2 = a_n1;
        a_n1 = a_n;
    }

    return a_n;
}
