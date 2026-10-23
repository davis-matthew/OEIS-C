/*
 * A131527  a(n) = 4*(n^1 + 1!)*(n^2 + 2!)*(n^3 + 3!)*(n^4 + 4!)*(n^5 +
 *          5!)/5!.
 * https://oeis.org/A131527
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^1 + 1!)*(n^2 + 2!)*(n^3 + 3!)*(n^4 + 4!)*(n^5 + 5!)/30
 * Range:     u8 none, u16 n <= 2, u32 n <= 5, u64 n <= 24, u128 n <= 464,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* factorials 1!..5! */
    static const seq_t fact[6] = {
        (seq_t)0,          /* placeholder for 0! */
        (seq_t)1,          /* 1!  */
        (seq_t)2,          /* 2!  */
        (seq_t)6,          /* 3!  */
        (seq_t)24,         /* 4!  */
        (seq_t)120         /* 5!  */
    };

    /* primes in the denominator 30 = 2 * 3 * 5 */
    static const int primes[3] = {2, 3, 5};

    seq_t product = (seq_t)1;
    seq_t pow     = (seq_t)n;   /* will hold n^i */
    int   denom   = 30;         /* remaining part of denominator */

    for (int i = 1; i <= 5; ++i) {
        /* term = n^i + i! */
        seq_t term = pow + fact[i];

        /* cancel as many factors of 2, 3, 5 as possible from term */
        for (int j = 0; j < 3; ++j) {
            int p = primes[j];
            while (denom % p == 0 && term % (seq_t)p == 0) {
                term /= (seq_t)p;
                denom /= p;
            }
        }

        product *= term;

        /* prepare next power of n (for next i) */
        if (i < 5)
            pow *= (seq_t)n;
    }

    /* any remaining denominator factor (1) is removed here */
    if (denom > 1)
        product /= (seq_t)denom;

    return product;
}
