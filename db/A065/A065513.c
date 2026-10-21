/*
 * A065513  Number of endofunctions of [n] with a cycle a->b->c->a and for all
 *          x in [n], some iterate f^k(x)=a.
 * https://oeis.org/A065513
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1) * (n-2) * n^(n-3) (for n ≥ 3)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A065513 */

seq_t a(idx_t n)
{
    /* For n < 3 the sequence is undefined; return 0. */
    if (n < 3)
        return (seq_t)0;

    /* Compute n^(n-3) by exponentiation by squaring */
    seq_t base = (seq_t)n;
    idx_t  exp  = n - 3;
    seq_t pow  = (seq_t)1;

    while (exp > 0) {
        if (exp & 1)
            pow *= base;
        base *= base;
        exp >>= 1;
    }

    /* Multiply by (n-1)*(n-2) */
    seq_t result = (seq_t)(n - 1) * (seq_t)(n - 2);
    result *= pow;

    return result;
}
