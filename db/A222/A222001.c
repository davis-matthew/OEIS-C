/*
 * A222001  Number of n X 3 arrays with each row a permutation of 1..3 having
 *          at least as many downsteps as the preceding row, with rows in
 *          lexicographically nonincreasing order.
 * https://oeis.org/A222001
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(n+3,3)+2
 * Range:     u8 n <= 9, u16 n <= 71, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* External linkage as required by the build system */
seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1. For n == 1 the value is 6. */
    seq_t result = 6;
    if (n <= 1)
        return result;

    for (idx_t i = 2; i <= n; ++i)
    {
        /* Compute the incremental increase:
           a(n) - a(n-1) = C(n+2,2) = (n+2)(n+1)/2.
           To avoid overflow in the product, divide the even factor
           by 2 before multiplication.  The even factor is always
           guaranteed to exist because (n+1)(n+2) is even. */
        seq_t t1 = (seq_t)i + 2;
        seq_t t2 = (seq_t)i + 1;
        if ((t1 & 1) == 0)
            t1 /= 2;
        else
            t2 /= 2;
        seq_t diff = t1 * t2;   /* exact binomial division */
        result += diff;
    }
    return result;
}
