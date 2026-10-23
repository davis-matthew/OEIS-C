/*
 * A187444  A trisection of A001405 (central binomial coefficients):
 *          binomial(3n+2,floor((3n+2)/2))/2, n >= 0.
 * https://oeis.org/A187444
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(3*n+2,floor((3*n+2)/2))/2
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes central binomial coefficient C(m,k) with 0 <= k <= m/2 using
   multiplicative formula.  The calculation is safe for 64‑bit
   seq_t values by performing the intermediate product in 128‑bit
   arithmetic when available. */
static seq_t binomial_multiplicative(idx_t m, idx_t k)
{
    if (k > m - k)                 /* use symmetry */
        k = m - k;

    seq_t result = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
#if defined(__SIZEOF_INT128__) && __SIZEOF_INT128__ == 16
        /* 128‑bit product to avoid intermediate overflow. */
        unsigned __int128 prod = (unsigned __int128)result * (m - k + i);
        result = (seq_t)(prod / i);
#else
        /* If 128‑bit not available, rely on the fact that the final
           result fits in seq_t and that seq_t supports / and * operators. */
        result = result * (m - k + i) / i;
#endif
    }
    return result;
}

seq_t a(idx_t n)
{
    /* 3*n+2 fits in idx_t.  m and k are used as indices for the binomial. */
    idx_t m = 3 * n + 2;

    /* floor((3*n+2)/2) */
    idx_t k = m / 2;

    seq_t val = binomial_multiplicative(m, k);
    return val / 2;
}
