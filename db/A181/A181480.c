/*
 * A181480  a(n) has generating function 1/((1-x)^k*(1-x^2)^(k*(k-1)/2)) for
 *          k=8.
 * https://oeis.org/A181480
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = coefficient of x^n in 1/((1-x)^8*(1-x^2)^28)
 *            (binomial‑sum representation) a(n) = Σ_{j=0}^{⌊n/2⌋}
 *            binom(7+n-2j, n-2j) * binom(27+j, j)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 17, u64 n <= 55, u128 n <= 274,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t mul_div(seq_t x, idx_t num, idx_t den)
{
    /* multiply by a small integer and divide by a small integer.
       Works for arbitrary‑precision seq_t as well as built‑in integer types. */
    return (x * (seq_t)num) / (seq_t)den;
}

static seq_t binom_7_plus(idx_t n)
{
    seq_t res = 1;
    for (idx_t k = 1; k <= n; ++k)
        res = mul_div(res, 7U + k, k);
    return res;
}

seq_t a(idx_t n)
{
    seq_t binom1 = binom_7_plus(n);   /* binom(7+n, n) */
    seq_t binom2 = 1;                 /* binom(27+0, 0) */
    seq_t sum   = 0;
    idx_t maxs  = n / 2;
    idx_t t     = n;

    for (idx_t s = 0; s <= maxs; ++s)
    {
        sum += binom1 * binom2;

        if (s == maxs)
            break;

        if (t >= 2)
        {
            binom1 = mul_div(binom1,
                             t * (t - 1),
                             (7 + t) * (7 + t - 1));
            t -= 2;
        }

        binom2 = mul_div(binom2, 27 + s + 1, s + 1);
    }

    return sum;
}
