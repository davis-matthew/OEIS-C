/*
 * A094826  Number of (s(0), s(1), ..., s(2n)) such that 0 < s(i) < 9 and
 *          |s(i) - s(i-1)| = 1 for i = 1,2,...,2n, s(0) = 1, s(2n) = 3.
 * https://oeis.org/A094826
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 36, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A094826: Number of latticed paths from (0,1) to (2n,3) staying in (0,9)
   with step size ±1.  Recurrence:
   a(n) = 6·a(n-1) – 9·a(n-2) + a(n-3),
   with a(1)=1, a(2)=3, a(3)=9.  The recurrence is computed with
   integer arithmetic only, avoiding overflow on the 64‑bit build by
   using only shifts and additions. */
seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for n=0; return 0 for safety */
        return (seq_t)0;
    if (n == 1)          /* a(1) */
        return (seq_t)1;
    if (n == 2)          /* a(2) */
        return (seq_t)3;
    if (n == 3)          /* a(3) */
        return (seq_t)9;

    /* a_1 = a(n-3), a_2 = a(n-2), a_3 = a(n-1) */
    seq_t a_1 = (seq_t)1;   /* a(1)  */
    seq_t a_2 = (seq_t)3;   /* a(2)  */
    seq_t a_3 = (seq_t)9;   /* a(3)  */

    for (idx_t i = 4; i <= n; ++i) {
        /* 6*a_3 == (a_3<<1) + (a_3<<2)   */
        seq_t t6 = (a_3 << 1) + (a_3 << 2);
        /* 9*a_2 == (a_2<<3) + a_2       */
        seq_t t9 = (a_2 << 3) + a_2;
        seq_t a_n = t6 - t9 + a_1;

        /* shift window for next iteration */
        a_1 = a_2;
        a_2 = a_3;
        a_3 = a_n;
    }
    return a_3;   /* now a_3 holds a(n) */
}
