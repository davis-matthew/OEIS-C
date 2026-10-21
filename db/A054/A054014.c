/*
 * A054014  Chowla function of n read modulo (the number of divisors of n).
 * https://oeis.org/A054014
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1039, u16 n <= 9555, u32 n <= 9555, u64 n <= 9555,
 *            u128 n <= 9555, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = (σ(n) - n - 1) mod τ(n)
 * where σ(n) is the sum of all divisors of n
 * & τ(n) is the number of divisors of n.
 * a(1) = 0 by definition (no proper divisors other than 1 and n).
 */
seq_t a(idx_t n)
{
    /* Guard for the special case n == 1 (and 0 if called). */
    if (n <= 1)
        return 0;

    idx_t tmp = n;          /* value to be factored */
    seq_t sigma = 1;        /* product of sums of prime powers: σ(n) */
    seq_t tau   = 1;        /* product of (exponent+1): τ(n)             */

    /* Trial division up to sqrt(tmp).  Use tmp/p to avoid overflow. */
    for (idx_t p = 2; p <= tmp / p; ++p)
    {
        if (tmp % p != 0)
            continue;

        /* Count the exponent of prime p in n. */
        int exp = 0;
        while (tmp % p == 0)
        {
            tmp /= p;
            ++exp;
        }

        /* Compute the factor (1 + p + p^2 + ... + p^exp). */
        seq_t term_pow = 1; /* p^0 */
        seq_t sum_factor = 0;
        for (int k = 0; k <= exp; ++k)
        {
            sum_factor += term_pow;
            term_pow *= (seq_t)p;
        }

        sigma *= sum_factor;            /* accumulate σ(n) */
        tau   *= (seq_t)(exp + 1);       /* accumulate τ(n) */
    }

    /* If a prime factor remains > 1. */
    if (tmp > 1)
    {
        seq_t pval = (seq_t)tmp;   /* the remaining prime */
        seq_t sum_factor = pval + (seq_t)1; /* 1 + p */
        sigma *= sum_factor;
        tau   *= (seq_t)2;       /* exponent is 1 */
    }

    /* Chowla's function: sum of divisors except 1 and n. */
    seq_t chowla = sigma - (seq_t)n - (seq_t)1;

    /* Result modulo τ(n). */
    return chowla % tau;
}
