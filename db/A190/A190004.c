/*
 * A190004  A190002/2.
 * https://oeis.org/A190004
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n + [n*(sinh(1))^2] + [n*(cosh(1))^2]) / 2
 * Range:     u8 n <= 107, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses the recurrence   d(n) = 2 + ⌊frac((n-1)*f) + f⌋
   where f = (cosh(2) – 3)/2 ≈ 0.38109784554181573, shared as the fractional
   part of both sinh²(1) and cosh²(1).  The value is accumulated from a(1)=2.  */

seq_t a(idx_t n)
{
    /* Return 0 (or any value) for indices below the offset; the sequence
       is defined for n ≥ 1.  */
    if (n == 0)
        return 0;

    /* Initial term for n = 1.  */
    seq_t result = 2;

    if (n == 1)
        return result;

    /* High‑precision rational approximation of f = (cosh(2)-3)/2.  */
    /* f ≈ 0.38109784554181573, represented as f_num / f_den.  */
    static const unsigned long long F_NUM = 381097845541815730ULL;   /* f_num */
    static const unsigned long long F_DEN = 1000000000000000000ULL;   /* 1e18  */

    /* Current fractional part of (k-1)*f, scaled by F_DEN.  */
    unsigned long long r = F_NUM;   /* r for k = 2 (i.e., (2-1)*f). */

    for (idx_t k = 2; k <= n; ++k) {
        /* Determine whether r + f ≥ 1.0 in scaled form.  */
        unsigned long long addOne = (r + F_NUM >= F_DEN) ? 1ULL : 0ULL;

        /* d(k) = 2 + addOne.  */
        result += (seq_t)(2 + addOne);

        /* Advance fractional part: r ← (r + f) mod 1.  */
        r += F_NUM;
        if (r >= F_DEN)
            r -= F_DEN;
    }

    return result;
}
