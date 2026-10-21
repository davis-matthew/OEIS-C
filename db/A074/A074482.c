/*
 * A074482  Consider the recursion b(1,n) = 1, b(k+1,n) = b(k,n) + (b(k,n)
 *          reduced mod(k+n)); then there is a number x such that b(k,n) -
 *          b(k-1,n) is a constant x depending only on n, for k > y =
 *          A074483(n). Sequence gives values of x.
 * https://oeis.org/A074482
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the first positive integer that eventually becomes the
 *            constant difference in the recurrence b(1,n) = 1, b(k+1,n) =
 *            b(k,n) + (b(k,n) mod (k+n)). For each n there is an index y =
 *            A074483(n) such that for k>y, b(k+1,n)-b(k,n) = a(n). The
 *            function below computes a(n) by iterating the recurre...
 * Range:     u8 none, u16 n <= 15, u32 n <= 37, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /*  b_k and the delta (difference) are both non‑negative */
    seq_t b = 1;                 /* b(1,n) */
    seq_t last_delta = 0;        /* placeholder */
    int  last_set = 0;           /* flag whether last_delta is valid */
    int  consec = 0;             /* consecutive equal deltas */

    for (idx_t k = 1; ; ++k)
    {
        /* b(k+1,n) = b + (b mod (k+n))   */
        seq_t mod_val = b % (seq_t)(k + n);
        seq_t next_b  = b + mod_val;
        seq_t delta   = mod_val;

        if (last_set && delta == last_delta)
            ++consec;
        else
        {
            consec = 1;
            last_delta = delta;
            last_set = 1;
        }

        /* Once we see the same delta for some small number
           of consecutive steps we assume it has stabilised. */
        if (consec >= 5)
            return delta;

        b = next_b;
    }
}
