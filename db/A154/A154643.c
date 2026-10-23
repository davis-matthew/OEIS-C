/*
 * A154643  a(n) is the number of spheres that are added in the n-th
 *          generation of Apollonian packing of three-dimensional spheres,
 *          starting with five mutually tangent spheres and using "strategy
 *          (b)" to count them (see the reference).
 * https://oeis.org/A154643
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  hard, more, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=5, a(1)=5, a(2)=20, a(3)=90,
   a(n) = 3*a(n-1) + 16*a(n-3) - 4*a(n-4)   for n ≥ 4 */

seq_t a(idx_t n)
{
    /* Handle the first four terms directly */
    if (n == 0u) return (seq_t)5;
    if (n == 1u) return (seq_t)5;
    if (n == 2u) return (seq_t)20;
    if (n == 3u) return (seq_t)90;

    /* Iterative evaluation of the recurrence
       a0,a1,a2,a3 represent a(n-4), a(n-3), a(n-2), a(n-1) respectively */
    seq_t a0 = (seq_t)5;   /* a(n-4) at start i = 4  */
    seq_t a1 = (seq_t)5;   /* a(n-3)      */
    seq_t a2 = (seq_t)20;  /* a(n-2)      */
    seq_t a3 = (seq_t)90;  /* a(n-1)      */

    for (idx_t i = 4u; i <= n; ++i)
    {
        seq_t cur = (seq_t)3 * a3 + (seq_t)16 * a1;   /* 3*a(n-1) + 16*a(n-3) */
        cur -= (seq_t)4 * a0;                       /* subtract 4*a(n-4)     */

        /* Shift window for next iteration */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = cur;
    }

    return a3;
}
