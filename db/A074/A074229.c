/*
 * A074229  Numbers n such that Kronecker(6,n)==mu(gcd(6,n)).
 * https://oeis.org/A074229
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-4) - a(n-5) with a(1)=1, a(2)=5, a(3)=19,
 *            a(4)=23, a(5)=25
 * Range:     u8 n <= 42, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the first five terms directly */
    if (n <= 5)
    {
        switch (n)
        {
            case 1: return (seq_t)1;
            case 2: return (seq_t)5;
            case 3: return (seq_t)19;
            case 4: return (seq_t)23;
            case 5: return (seq_t)25;
            default: return (seq_t)0;  /* unreachable for n>=1 */
        }
    }

    /* recurrence uses the five most recent values:
       a(k-5), a(k-4), a(k-3), a(k-2), a(k-1)  → stored as a[0]..a[4] */
    seq_t a[5] = { (seq_t)1, (seq_t)5, (seq_t)19, (seq_t)23, (seq_t)25 };

    for (idx_t i = 6; i <= n; ++i)
    {
        seq_t next = a[4] + a[1] - a[0];  // a(i-1) + a(i-4) - a(i-5)

        /* shift the history left by one position */
        a[0] = a[1];
        a[1] = a[2];
        a[2] = a[3];
        a[3] = a[4];
        a[4] = next;
    }

    return a[4];   /* the most recent value a(n) */
}
