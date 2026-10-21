/*
 * A201971  a(n) is the largest m such that n is congruent to -2, -1, 0, 1 or
 *          2 mod k for all k from 1 to m.
 * https://oeis.org/A201971
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = largest m such that n ≡ -2,-1,0,1,2 (mod k) for all 1 ≤ k
 *            ≤ m
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function works for all n ≥ 3; for n ≤ 2 it returns 0. */

seq_t a(idx_t n)
{
    if (n <= 2)
        return (seq_t)0;   /* offset starts at 3, behaviour below 3 is undefined */

    idx_t k;
    for (k = 1;; ++k)
    {
        /* For k = 1 or 2 the condition is always satisfied. */
        if (k <= 2)
            continue;

        idx_t r = n % k;
        /* Fail if remainder is neither within 2 of 0 nor within 2 of k. */
        if (r > 2 && r < k - 2)
            break;
    }
    return (seq_t)(k - 1);
}
