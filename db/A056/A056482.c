/*
 * A056482  Number of primitive (aperiodic) palindromic structures using
 *          exactly three different symbols.
 * https://oeis.org/A056482
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 20, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A056482 – Number of primitive (aperiodic) palindromic structures
 *  using exactly three different symbols.
 *
 *  Formula: a(n) = Σ_{d|n} μ(d) *
 *              (3^k - 3·2^k + 3) / 6          with k = ceil((n/d)/2)
 *  (k = floor((n/d)/2) + (n/d mod 2))
 *
 *  The implementation follows this formula directly.
 */

static seq_t power_seq_t(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp > 0)
    {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

static seq_t stirling3(idx_t k)
{
    /* S(k,3) = (3^k - 3·2^k + 3) / 6 for k >= 1 */
    seq_t pow3 = power_seq_t(3, k);
    seq_t pow2 = power_seq_t(2, k);
    seq_t num  = pow3 - (seq_t)3 * pow2 + (seq_t)3;
    return num / (seq_t)6;
}

/* Möbius function μ(n) using trial division */
static int mobius(idx_t n)
{
    if (n == 1) return 1;
    int cnt = 0;
    for (idx_t p = 2; p * p <= n; ++p)
    {
        if (n % p == 0)
        {
            int exp = 0;
            while (n % p == 0)
            {
                n /= p;
                ++exp;
                if (exp > 1) return 0;   /* squared prime factor */
            }
            ++cnt;
        }
    }
    if (n > 1) ++cnt;               /* remaining prime factor > sqrt */
    return (cnt & 1) ? -1 : 1;
}

/* Return the value a(n) */
seq_t a(idx_t n)
{
    /* Collect divisors of n */
    idx_t divs[200];   /* 200 is ample for n ≤ 10000 */
    uint32_t div_count = 0;

    for (idx_t d = 1; d * d <= n; ++d)
    {
        if (n % d == 0)
        {
            divs[div_count++] = d;
            if (d != n / d) divs[div_count++] = n / d;
        }
    }

    seq_t total = 0;
    for (uint32_t i = 0; i < div_count; ++i)
    {
        idx_t d       = divs[i];
        idx_t m       = n / d;                     /* length of the potential block */
        idx_t k       = m / 2 + (m & 1);           /* number of free positions in a palindrome of length m */
        seq_t term    = stirling3(k);
        int mu        = mobius(d);
        if (mu == 1)   total += term;
        else if (mu == -1) total -= term;
        /* mu == 0: skip */
    }

    return total;
}
