/*
 * A069897  Integer quotient of the largest and the smallest prime factors of
 *          n, with a(1) = 1.
 * https://oeis.org/A069897
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, look
 * Formula:   a(n) = floor( A006530(n) / A020639(n) )
 * Range:     u8 n <= 1040, u16 n <= 9896, u32 n <= 9896, u64 n <= 9896,
 *            u128 n <= 9896, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   integer quotient of the largest and the smallest prime factors of n,
    with a(1) = 1. */
seq_t a(idx_t n)
{
    /* Handle n = 1 explicitly as per OEIS definition */
    if (n == 1)
        return (seq_t)1;

    seq_t min_pf = (seq_t)n;   /* initialize to n itself */
    seq_t max_pf = (seq_t)1;
    idx_t  t     = n;          /* use idx_t for trial division to avoid overflows */

    /* Trial division up to sqrt(t) */
    for (idx_t p = 2; p <= t / p; ++p) {
        if (t % p == 0) {
            /* p is a prime divisor */
            if (p < min_pf) min_pf = (seq_t)p;
            if (p > max_pf) max_pf = (seq_t)p;
            /* Remove all instances of p */
            while (t % p == 0)
                t /= p;
        }
    }

    /* If t > 1, it is a prime factor larger than sqrt(original n) */
    if (t > 1) {
        seq_t last_pf = (seq_t)t;
        if (last_pf < min_pf) min_pf = last_pf;
        if (last_pf > max_pf) max_pf = last_pf;
    }

    return max_pf / min_pf;
}
