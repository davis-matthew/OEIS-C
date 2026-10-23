/*
 * A178459  Partial sums of floor(2^n/31).
 * https://oeis.org/A178459
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-5) + 2^(n-4) - 1, for n > 4; a(1)=a(2)=a(3)=a(4)=0
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 35, u64 n <= 67, u128 n <= 131,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 4)
        return 0;
    /* a[0] corresponds to a(0)=0, a[1..4] are a(1..4)=0. */
    seq_t a_mod[5] = {0, 0, 0, 0, 0};
    /* pow2 holds 2^(i-4) for current i, starting with i=5 -> 2^1 = 2 */
    seq_t pow2 = 2;
    for (idx_t i = 5; i <= n; ++i)
    {
        /* Because (i-5) % 5 == i % 5, the old value in a_mod[i%5]
           is a(i-5). */
        a_mod[i % 5] = a_mod[i % 5] + pow2 - 1;
        /* Prepare pow2 for i+1 : multiply by 2.  The shift is safe
           because we only multiply while the result still fits in seq_t
           when the caller's requested value fits. */
        pow2 = pow2 << 1;
    }
    return a_mod[n % 5];
}
