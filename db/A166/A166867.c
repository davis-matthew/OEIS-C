/*
 * A166867  a(n) = Pell(n+3) - Jacobsthal(n+4).
 * https://oeis.org/A166867
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*a(n-1)+a(n-2)-5*a(n-3)-2*a(n-4) Initial values: a(0)=0,
 *            a(1)=1, a(2)=8, a(3)=27
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 23, u64 n <= 48, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;
    if (n == 2) return 8;
    if (n == 3) return 27;

    /* Compute iteratively using the recurrence. */
    seq_t a0 = 0;  /* a(n-4) */
    seq_t a1 = 1;  /* a(n-3) */
    seq_t a2 = 8;  /* a(n-2) */
    seq_t a3 = 27; /* a(n-1) */
    seq_t an;      /* a(n) */

    for (idx_t i = 4; i <= n; ++i) {
        an = 3 * a3 + a2 - 5 * a1 - 2 * a0;
        /* shift window: a0<-a1, a1<-a2, a2<-a3, a3<-an */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return a3;
}
