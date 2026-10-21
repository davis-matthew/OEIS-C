/*
 * A080835  E.g.f.: exp( x/( 1 - x - x^2 - x^3 ) ).
 * https://oeis.org/A080835
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A080835
    a(0) = 1, a(1) = 1, a(2) = 3, a(3) = 19, a(4) = 169, a(5) = 1761
    a(n) = (2n-1)a(n-1) + (n-2)(n-1)a(n-2) + (n-2)(n-1)a(n-3)
           -(n-3)(n-2)(n-1)(3n-14)a(n-4)
           -2(n-5)(n-4)(n-3)(n-2)(n-1)a(n-5)
           -(n-6)(n-5)(n-4)(n-3)(n-2)(n-1)a(n-6)   for n >= 6   */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* base values for n = 0 .. 5 */
    static const seq_t base[6] = { 1, 1, 3, 19, 169, 1761 };

    /* static memoisation table */
    static seq_t *dp = NULL;
    static idx_t dp_size = 0;     /* number of elements in dp */

    if (n < 6)                       /* return from the base table */
        return base[(size_t)n];

    /* expand the table if we need a larger index */
    if (dp_size <= n) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(dp, new_size * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);   /* allocation failed */
        dp = tmp;

        /* initialise the first six elements on the very first allocation */
        if (dp_size == 0) {
            for (size_t i = 0; i < 6; ++i)
                dp[i] = base[i];
        }

        /* compute only the indices that have not been filled yet */
        size_t start = (dp_size > 6) ? (size_t)dp_size : 6;
        for (size_t i = start; i <= n; ++i) {
            seq_t n_seq = (seq_t)i;

            seq_t t1 = ((seq_t)2 * n_seq - (seq_t)1) * dp[i - 1];
            seq_t t2 = (n_seq - (seq_t)2) * (n_seq - (seq_t)1) * dp[i - 2];
            seq_t t3 = (n_seq - (seq_t)2) * (n_seq - (seq_t)1) * dp[i - 3];
            seq_t t4 = (n_seq - (seq_t)3) * (n_seq - (seq_t)2) *
                       (n_seq - (seq_t)1) * ((seq_t)3 * n_seq - (seq_t)14) *
                       dp[i - 4];
            seq_t t5 = (seq_t)2 * (n_seq - (seq_t)5) * (n_seq - (seq_t)4) *
                       (n_seq - (seq_t)3) * (n_seq - (seq_t)2) *
                       (n_seq - (seq_t)1) * dp[i - 5];
            seq_t t6 = (n_seq - (seq_t)6) * (n_seq - (seq_t)5) *
                       (n_seq - (seq_t)4) * (n_seq - (seq_t)3) *
                       (n_seq - (seq_t)2) * (n_seq - (seq_t)1) *
                       dp[i - 6];

            dp[i] = t1 + t2 + t3 - t4 - t5 - t6;
        }

        dp_size = (idx_t)new_size;   /* remember the new table size */
    }

    return dp[(size_t)n];
}
