/*
 * A051879  Partial sums of A051798.
 * https://oeis.org/A051879
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(n+4,4)*(9*n+5)/5 = ((n+4)(n+3)(n+2)(n+1)(9*n+5))/120
 * Range:     u8 n <= 3, u16 n <= 13, u32 n <= 139, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t gcd_seq_t(seq_t a, seq_t b)
{
    /* Euclidean algorithm for seq_t */
    while (b != 0)
    {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    /* The denominator 120 = 5! * 5 */
    seq_t denom = 120;
    seq_t result = 1;

    /* Factors of the numerator */
    seq_t fac[5];
    fac[0] = nn + 4;
    fac[1] = nn + 3;
    fac[2] = nn + 2;
    fac[3] = nn + 1;
    fac[4] = 9 * nn + 5;

    for (int i = 0; i < 5; ++i)
    {
        seq_t f = fac[i];
        seq_t g = gcd_seq_t(f, denom);  /* cancel common factor with the denominator */
        denom /= g;
        f /= g;
        result *= f;                    /* no overflow for n ≤ 10000 (64‑bit build) */
    }

    /* denom should now be 1; result holds a(n) */
    return result;
}
