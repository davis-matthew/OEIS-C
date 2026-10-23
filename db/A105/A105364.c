/*
 * A105364  Rearrangement of positive integers according to parity of
 *          (prime(n)-1)/2 (see comments).
 * https://oeis.org/A105364
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) from OEIS A105364
 *
 *  The terms are produced by repeatedly selecting the next unused odd
 *  or even integer.  For step n (1‑based) we look at the prime number
 *  prime(n+1).  Let x = (prime(n+1) – 1)/2.  If x is odd the next
 *  unused even number is chosen; if x is even the next unused odd
 *  number is chosen.  The chosen integer is the value of a(n).
 *
 *  The implementation recomputes the sequence from scratch for each
 *  call, so calls may be made in any order.  It uses the helper
 *  function get_prime from "../utils/get_prime.h".
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    seq_t next_odd  = 1;   /* next unused odd number   */
    seq_t next_even = 2;   /* next unused even number  */
    seq_t res       = 0;   /* placeholder for result   */

    for (idx_t i = 1; i <= n; ++i) {
        seq_t prime = get_prime(i + 1);          /* prime(i+1)   */
        seq_t parity = ((prime - 1) / 2) & (seq_t)1; /* 0 = even, 1 = odd */

        if (parity) {             /* odd -> use next even */
            res = next_even;
            next_even += 2;
        } else {                  /* even -> use next odd */
            res = next_odd;
            next_odd += 2;
        }
    }
    return res;
}
