/*
 * A014458  Theta series of quadratic form with Gram matrix [ 2, 1, 0; 1, 4,
 *          1; 0, 1, 2 ].
 * https://oeis.org/A014458
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer solutions (x,y,z) to x^2 + y^2 + 3*z^2
 *            = 2*n. Equivalent to the coefficient of q^(2n) in theta3(q)^2 *
 *            theta3(q^3)
 * Range:     u8 n <= 265, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count representations of m as a sum of two squares.  Uses the
   classical theorem: if m has any prime factor ≡3 (mod 4) to an
   odd exponent, the count is 0; otherwise the count is
   4 * Π (e_i+1) over primes p_i ≡1 (mod 4). */
static seq_t r2_value(idx_t m)
{
    if (m == 0) return 1;

    seq_t prod = 1;
    idx_t tmp = m;

    for (idx_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            idx_t e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            if (p % 4 == 3) {
                if (e & 1)  /* odd exponent of a 3 (mod 4) prime */
                    return 0;
            } else if (p % 4 == 1) {
                prod *= ((seq_t)e + 1);
            }
            /* primes 2 or ≡2 (mod 4) don't affect the count */
        }
    }
    if (tmp > 1) {          /* remaining prime factor */
        if (tmp % 4 == 3)   /* odd exponent of 3 (mod 4) prime */
            return 0;
        if (tmp % 4 == 1)
            prod *= 2;     /* exponent 1 → factor (e+1)=2 */
    }

    return prod * 4;
}

/* Integer square root (floor).  Generic for idx_t values. */
static idx_t isqrt_idx(idx_t n)
{
    idx_t low = 0, high = (n >> 1) + 1;
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        __int128 sq = (__int128)mid * mid;
        if (sq == n) return mid;
        if (sq < n)  low = mid + 1;
        else         high = mid - 1;
    }
    return high;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;

    /* maximum |z| such that 3*z^2 <= 2*n */
    idx_t maxZ = isqrt_idx((idx_t)(((__int128)n * 2) / 3));

    for (idx_t z = 0; z <= maxZ; ++z) {
        idx_t m = ((idx_t)2 * n) - ( (__int128)3 * z * z );
        seq_t r2 = r2_value(m);
        if (z == 0)
            sum += r2;
        else
            sum += r2 * 2;      /* ±z */
    }

    return sum;
}
