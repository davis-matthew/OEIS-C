/*
 * A239141  Number of strict partitions of n having standard deviation <= 1.
 * https://oeis.org/A239141
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1, a(2)=1, a(3)=2; For n ≥ 4: a(n) = 3 if n % 3 == 0,
 *            otherwise a(n) = 2. This follows from the generating function
 *            G(x)=-(x^6+x^5+x^4+2x^3+x^2+x)/(x^3-1), giving the recurrence
 *            a(n)=a(n-3)+c_n, where c_n=1,1,2,1,1,1 for n=1..6 and c_n=0 for
 *            n≥7. Hence a(n)=a(n-3) for n≥7, yielding the 3‑per...
 * Range:     u8 n <= 8922, u16 n <= 8922, u32 n <= 8922, u64 n <= 8922,
 *            u128 n <= 8922, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)            /* offset is 1, but handle n==0 gracefully */
        return (seq_t)0;

    if (n == 1 || n == 2)
        return (seq_t)1;

    if (n == 3)
        return (seq_t)2;

    /* For n ≥ 4 */
    if ((n % 3u) == 0u)
        return (seq_t)3;

    return (seq_t)2;
}
