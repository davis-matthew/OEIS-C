/*
 * A017125  Table read by antidiagonals of Fibonacci-type sequences.
 * https://oeis.org/A017125
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A017125
    Definition:  a(n) is the entry at position n in the table
    T(n,k) with T(n,0)=n, T(n,1)=1 and T(n,k)=T(n,k-1)+T(n,k-2)
    read by antidiagonals (0,0),(0,1),(1,0),(0,2),(1,1),(2,0),… */
seq_t a(idx_t n)
{
    /* find the antidiagonal number s such that
       triangle(s-1) <= n < triangle(s)  with  triangle(t)= (t+1)(t+2)/2 */
    idx_t s = 0;
    for (/*empty*/; (unsigned __int128)(s+1)*(s+2)/2 <= n; ++s)
        ;
    /* number of terms before this diagonal */
    idx_t prev = s*(s+1)/2;
    /* offset inside the diagonal */
    idx_t t = n - prev;
    /* coordinates (row, column) in the table */
    idx_t nprime = t;      /* row index */
    idx_t kprime = s - t;  /* column index */

    /* T(nprime,0)=nprime */
    if (kprime == 0)
        return (seq_t)nprime;

    /* compute Fibonacci numbers up to kprime */
    seq_t f0 = 0;   /* F_0 */
    seq_t f1 = 1;   /* F_1 */
    for (idx_t i = 2; i <= kprime; ++i) {
        seq_t tmp = f0 + f1;
        f0 = f1;
        f1 = tmp;
    }
    /* after loop: f0 = F_{kprime-1}, f1 = F_{kprime} */
    seq_t fk        = f1;            /* F_kprime */
    seq_t fk_minus1 = f0;            /* F_{kprime-1} */
    return fk + (seq_t)nprime * fk_minus1;
}
