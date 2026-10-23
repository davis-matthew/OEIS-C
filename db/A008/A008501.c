/*
 * A008501  7-dimensional centered tetrahedral numbers.
 * https://oeis.org/A008501
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binom(n+8,8) – binom(n,8)
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 58, u64 n <= 1419,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  where binom(k,8) is the binomial coefficient C(k,8).  */
/* This expression is equivalent to the polynomial formula in the OEIS entry.  */

static inline seq_t binom8(idx_t m)
{
    /* return C(m,8);  m is a 64‑bit index. */
    if (m < 8) return (seq_t)0;

    /* Compute the product multiplicatively using a 128‑bit intermediate to avoid overflows. */
    __uint128_t r = 1;
    for (idx_t i = 1; i <= 8; ++i)
    {
        r = r * (m - 8 + i);  /* multiply by the next numerator term */
        r /= i;               /* divide by the denominator term; the division is exact */
    }
    return (seq_t)r;
}

seq_t a(idx_t n)
{
    seq_t high = binom8(n + 8);   /* C(n+8,8) */
    seq_t low  = binom8(n);       /* C(n,8)  */
    return high - low;            /* difference gives a(n) */
}
