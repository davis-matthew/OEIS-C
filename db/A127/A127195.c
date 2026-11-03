/*
 * A127195  Number of subsets of {1,2,...,n} which contain no three
 *          consecutive odd numbers.
 * https://oeis.org/A127195
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Tribonacci(floor((n+1)/2)+3) * 2^floor(n/2)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Validated: partial, pass, 32 terms through n = 32 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t trib(idx_t k)
{
    /* Tribonacci numbers: T(0)=0, T(1)=0, T(2)=1. */
    if (k == 0 || k == 1)
        return (seq_t)0;
    if (k == 2)
        return (seq_t)1;

    seq_t t0 = (seq_t)0;   /* T(i-3)   */
    seq_t t1 = (seq_t)0;   /* T(i-2)   */
    seq_t t2 = (seq_t)1;   /* T(i-1)   */
    seq_t t3;

    for (idx_t i = 3; i <= k; ++i) {
        t3 = t2 + t1 + t0; /* T(i)     */
        t0 = t1;
        t1 = t2;
        t2 = t3;
    }
    return t2; /* T(k) */
}

seq_t a(idx_t n)
{
    /* number of odd positions */
    idx_t t = (n + 1) / 2;  /* ceil(n/2) */
    /* number of even positions */
    idx_t e = n / 2;        /* floor(n/2) */

    seq_t trib_val = trib(t + 3);    /* T(ceil(n/2)+3) */
    seq_t pow2 = (seq_t)1;
    /* compute 2^e by left shift */
    for (idx_t i = 0; i < e; ++i)
        pow2 <<= 1;

    return trib_val * pow2;
}
