/*
 * A085292  Product of Lucas (A000204) and a Pell companion series (A001333).
 * https://oeis.org/A085292
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1)+7*a(n-2)+2*a(n-3)-a(n-4) for n ≥ 5; initial
 *            values: a(1)=1, a(2)=9, a(3)=28, a(4)=119
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the predefined indices directly. */
    if (n == 0)
        return (seq_t)0;
    if (n == 1)
        return (seq_t)1;
    if (n == 2)
        return (seq_t)9;
    if (n == 3)
        return (seq_t)28;
    if (n == 4)
        return (seq_t)119;

    /* For n ≥ 5, use the recurrence relation. */
    seq_t a1 = (seq_t)1;   /* a(n-4) */
    seq_t a2 = (seq_t)9;   /* a(n-3) */
    seq_t a3 = (seq_t)28;  /* a(n-2) */
    seq_t a4 = (seq_t)119; /* a(n-1) */

    for (idx_t i = 5; i <= n; ++i) {
        seq_t an = (seq_t)2 * a4 + (seq_t)7 * a3 + (seq_t)2 * a2 - a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = an;
    }
    return a4;  /* a(n) */
}
