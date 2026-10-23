/*
 * A166778  Number of nX2 1..5 arrays containing at least one of each value,
 *          all equal values connected, and rows considered as a single number
 *          in nondecreasing order.
 * https://oeis.org/A166778
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/2520)*(n-1)*(n-2)*(n^6 + 35*n^5 + 551*n^4 + 4033*n^3 +
 *            8106*n^2 - 11790*n + 1260)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Offset: 1 (a(1) = 0) */
seq_t a(idx_t n)
{
    /* For n < 3 the combinatorial construction is impossible. */
    if (n < 3)
        return 0;

    /* Use 128‑bit arithmetic to hold intermediate results safely. */
#if defined(__SIZEOF_INT128__)
    __uint128_t N  = ( __uint128_t ) n;
    __uint128_t N2 = N * N;
    __uint128_t N3 = N2 * N;
    __uint128_t N4 = N3 * N;
    __uint128_t N5 = N4 * N;
    __uint128_t N6 = N5 * N;

    /* Compute the polynomial part */
    __uint128_t P = N6
                  + (__uint128_t)35 * N5
                  + (__uint128_t)551 * N4
                  + (__uint128_t)4033 * N3
                  + (__uint128_t)8106 * N2
                  - (__uint128_t)11790 * N
                  + 1260;

    __uint128_t num = (N - 1) * (N - 2) * P;
    return (seq_t)(num / 2520);
#else
    /* If 128–bit integers are not available, fall back to manual multiplication.
       This path works for all unsigned integer types, but is a little slower. */
    seq_t nn  = (seq_t)n;
    seq_t n2  = nn * nn;
    seq_t n3  = n2 * nn;
    seq_t n4  = n3 * nn;
    seq_t n5  = n4 * nn;
    seq_t n6  = n5 * nn;

    seq_t P = n6
            + (seq_t)35 * n5
            + (seq_t)551 * n4
            + (seq_t)4033 * n3
            + (seq_t)8106 * n2
            - (seq_t)11790 * nn
            + 1260;

    seq_t prod = (seq_t)(nn - 1) * (seq_t)(nn - 2) * P;
    return prod / 2520;
#endif
}
