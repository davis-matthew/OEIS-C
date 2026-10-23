/*
 * A155803  A023001 interleaved with 2*A023001 and 4*A023001.
 * https://oeis.org/A155803
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(2^n / 7)
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 34, u64 n <= 66, u128 n <= 130,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent recurrence:  a(0)=a(1)=a(2)=0, a(3)=1, 
   a(n)=2*a(n-1)+a(n-3)-2*a(n-4)   for n>=4 */

/* Compute a(n) for any n>=0.  The recurrence uses only
   additions, subtractions and binary multiplications by 2,
   so it works with all supported seq_t types (including
   arbitrary‑precision).  For n where the result fits in seq_t
   (e.g. n<=66 for 64‑bit seq_t) the value is exact; larger
   n produce overflow as allowed. */
seq_t a(idx_t n)
{
    if (n < 3u) return 0;
    /* initialise a(0)=a(1)=a(2)=0, a(3)=1 */
    seq_t a0 = 0;  /* a(n-4)   */
    seq_t a1 = 0;  /* a(n-3)   */
    seq_t a2 = 0;  /* a(n-2)   */
    seq_t a3 = 1;  /* a(n-1)   */
    if (n == 3u) return a3;

    seq_t res = 0;
    for (idx_t i = 4u; i <= n; ++i) {
        /* a(n) = 2*a(n-1) + a(n-3) - 2*a(n-4) */
        res = 2 * a3 + a1 - 2 * a0;
        /* shift window: a0<-a1, a1<-a2, a2<-a3, a3<-res */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = res;
    }
    return res;
}
