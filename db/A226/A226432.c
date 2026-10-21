/*
 * A226432  The number of simple permutations of length n in a particular
 *          geometric grid class.
 * https://oeis.org/A226432
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1, a(2) = 2, a(3) = 0, a(4) = 2, a(5) = 3, a(6) = 7 a(n)
 *            = 2*a(n-1) + a(n-2) - 2*a(n-3) - a(n-4) for n >= 7
 * Range:     u8 n <= 11, u16 n <= 22, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This follows from the rational generating function
   g(x) = (x - 5x^3 + 2x^4 + 4x^5 + x^6) / (1 - 2x - x^2 + 2x^3 + x^4). */

seq_t a(idx_t n)
{
    static const seq_t init[6] = {
        (seq_t)1, (seq_t)2, (seq_t)0,
        (seq_t)2, (seq_t)3, (seq_t)7
    };

    if (n == 0) /* offset starts at 1, undefined for n==0 */
        return (seq_t)0;

    if (n <= 6)
        return init[n - 1];

    /* Sliding window: a_{k-3}, a_{k-2}, a_{k-1}, a_k */
    seq_t a_m3 = init[2]; /* a(3) */
    seq_t a_m2 = init[3]; /* a(4) */
    seq_t a_m1 = init[4]; /* a(5) */
    seq_t a_m0 = init[5]; /* a(6) */

    for (idx_t i = 7; i <= n; ++i) {
        seq_t next = (a_m0 * (seq_t)2 + a_m1) -
                     (a_m2 * (seq_t)2 + a_m3);
        a_m3 = a_m2;
        a_m2 = a_m1;
        a_m1 = a_m0;
        a_m0 = next;
    }
    return a_m0;
}
