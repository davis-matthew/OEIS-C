/*
 * A089643  3^a(n) divides C(3n,n); 3-adic valuation of A005809.
 * https://oeis.org/A089643
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = v_3( binom(3n, n) ) = Σ_{k≥1} ( ⌊3n/3^k⌋ – ⌊n/3^k⌋ –
 *            ⌊2n/3^k⌋ ) Computed in O(log_3 n) by iterating over powers of 3
 * Range:     u8 n <= 9544, u16 n <= 9544, u32 n <= 9544, u64 n <= 9544,
 *            u128 n <= 9544, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use 128‑bit integers for intermediate products to avoid overflow. */
    __uint128_t n3 = (__uint128_t)3 * ((__uint128_t)n);
    __uint128_t n2 = (__uint128_t)2 * ((__uint128_t)n);
    __uint128_t p  = 3;            /* current power of 3, as 128‑bit integer */
    seq_t       res = 0;

    while (p <= n3)
    {
        /* Compute the floor divisions using 128‑bit arithmetic. */
        seq_t term = (seq_t)(n3 / p);
        term -= (seq_t)(n3 / (p * 3));   /* n/p for current power: use n3/p*3? */
        term -= (seq_t)(n2 / p);

        /* The above approach is incorrect for the second subtraction.  We need
           floor(n / p) and floor(2n / p) directly.  Since n3 = 3n, we can
           get floor(n / p) by floor(3n / (3p)) = floor(n3 / (p*3)).       */

        res += term;

        /* Move to the next power of 3, guarding against overflow of 'p'. */
        if (p > ( ( (__uint128_t)-1 ) / 3 ))
            break;
        p *= 3;
    }
    return res;
}
