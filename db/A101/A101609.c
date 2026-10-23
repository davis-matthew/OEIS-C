/*
 * A101609  a(n) = n! * Sum_{k=1..floor(n/2)} 1/k.
 * https://oeis.org/A101609
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(1) = 0
 * a(n) = a(n-1) * n + (1 + (-1)^n) * (n-1)!
 *        = a(n-1) * n + (n even ? 2*(n-1)! : 0)
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n < 1) return 0;                /* undefined for n < 1, just return 0 */
    seq_t a_val = 0;                    /* a(1) */
    seq_t fact  = 1;                    /* (n-1)!  for n=2 and onward */

    /* loop from 2 up to n */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t i_seq = (seq_t)i;
        a_val = a_val * i_seq;          /* a(n-1) * n */

        /* if n is even, add 2*(n-1)! */
        if ((i & 1) == 0) {
            a_val += (seq_t)2 * fact;
        }

        fact *= i_seq;                  /* update factorial: (n)! for next iteration */
    }

    return a_val;
}
