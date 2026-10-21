/*
 * A065888  a(n) = number of endofunctions on [n] with a 4-cycle a->b->c->d->a
 *          and for any x in [n], some iterate f^k(x) = a.
 * https://oeis.org/A065888
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1)*(n-2)*(n-3)*n^(n-4) (offset 4)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 10, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t seq_pow_seq_t(seq_t base, idx_t exp)
{
    /* binary exponentiation with seq_t arithmetic */
    seq_t result = 1;
    while (exp)
    {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n < 4)
        return 0;                 /* values below the offset are irrelevant */

    /* (n-1)*(n-2)*(n-3) */
    seq_t prod = (seq_t)(n - 1);
    prod *= (seq_t)(n - 2);
    prod *= (seq_t)(n - 3);

    /* n^(n-4) */
    seq_t pow = seq_pow_seq_t((seq_t)n, n - 4);

    return prod * pow;
}
