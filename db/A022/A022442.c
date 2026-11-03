/*
 * A022442  a(n) = c(n) + c(n-1) where c is the sequence of numbers not in a.
 * https://oeis.org/A022442
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 2; * for n>0, a(n) = b(n) where * b(0) = 0, * b(2n) =
 *            -b(n) + 9n + 3, * b(2n+1) = -b(n) + 9n + 6 - 2[n==0]. * The
 *            recurrence is evaluated recursively
 * Range:     u8 n <= 84, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 62 terms through n = 9890 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t b_rec(idx_t n)
{
    if (n == 0) return 0;                 /* base case: b(0)=0          */
    idx_t k  = n >> 1;                    /* k = floor(n/2)              */
    seq_t sub = b_rec(k);                 /* recursive call for b(k)     */

    /* Choose the constant part (3,4,or6) */
    seq_t add = (seq_t)((n & 1) == 0 ? 3          /* even n */
                                   : ((k == 0) ? 4  /* n==1 case   */
                                                : 6));/* odd n>1    */

    /* Compute 9*k using idx_t to avoid overflow for usual limits   */
    idx_t prod = 9 * k;
    seq_t nine_k = (seq_t) prod;

    return nine_k + add - sub;           /* = 9k + add - b(k)           */
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)2;          /* first term as given in OEIS */
    return b_rec(n);                      /* use recurrence for n>0      */
}
