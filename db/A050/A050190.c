/*
 * A050190  T(n,5), array T as in A050186; a count of aperiodic binary words.
 * https://oeis.org/A050190
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*floor(binomial(n-1,4)/5)
 * Range:     u8 n <= 10, u16 n <= 25, u32 n <= 222, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the term a(n) of OEIS sequence A050190.
   n is the index (idx_t). The sequence is defined for n ≥ 5.
   For n < 5 the function returns 0. */
seq_t a(idx_t n)
{
    if (n < 5) return 0;

    /* Compute C(n-1,4) = (n-1)(n-2)(n-3)(n-4)/24  */
    unsigned __int128 comb = 1;
    idx_t a0 = n - 1;          /* a0 = n-1  */
    for (int i = 1; i <= 4; ++i) {
        comb = comb * (a0 - i + 1) / i;   /* exact division at each step */
    }

    /* Floor division by 5 */
    unsigned __int128 q = comb / 5;
    /* Multiply by n to get the term */
    unsigned __int128 res = (__uint128_t)n * q;

    return (seq_t)res;
}
