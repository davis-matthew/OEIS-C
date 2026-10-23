/*
 * A027009  a(n) = Sum_{k=floor((n+2)/2)..n} T(n, k), T given by A026998.
 * https://oeis.org/A027009
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) satisfies a(n) = 2*a(n-1) + 3*a(n-2) - 6*a(n-3) - a(n-4) +
 *            2*a(n-5)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Initial values (offset 1): a(1)=1, a(2)=1, a(3)=9, a(4)=14, a(5)=46 */

seq_t a(idx_t n)
{
    /* Handle the offset: the first requested value is n==1. */
    if (n == 0) return 0;          /* undefined for n<1, return 0 for safety */

    /* Base cases */
    switch (n) {
        case 1: return (seq_t)1;
        case 2: return (seq_t)1;
        case 3: return (seq_t)9;
        case 4: return (seq_t)14;
        case 5: return (seq_t)46;
    }

    /* Recurrence requires the 5 previous terms.
       a0 = a(0) = -1  (needed for the recurrence at n=5)    */
    seq_t a0 = (seq_t)-1;       /* -1 in unsigned arithmetic gives all bits set */
    seq_t a1 = (seq_t)1;        /* a(1) */
    seq_t a2 = (seq_t)1;        /* a(2) */
    seq_t a3 = (seq_t)9;        /* a(3) */
    seq_t a4 = (seq_t)14;       /* a(4) */
    seq_t a5 = (seq_t)46;       /* a(5) */

    for (idx_t i = 6; i <= n; ++i) {
        /* Compute a(i) = 2*a5 + 3*a4 - 6*a3 - a2 + 2*a1  */
        seq_t next = 2 * a5 + 3 * a4 - 6 * a3 - a2 + 2 * a1;

        /* Shift the window for the next iteration */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = next;
    }

    return a5;   /* a5 now holds a(n) */
}
