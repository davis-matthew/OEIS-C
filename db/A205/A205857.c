/*
 * A205857  Numbers k for which 6 divides s(k)-s(j) for some j<k; each k
 *          occurs once for each such j; s(k) denotes the (k+1)-st Fibonacci
 *          number.
 * https://oeis.org/A205857
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th integer k for which there exists j<k with 6 |
 *            (Fib(k+1) – Fib(j+1)). Each such pair (k,j) contributes exactly
 *            one occurrence of k. Implemented by keeping counts of Fibonacci
 *            residues modulo 6. Search is O(n) time and O(1) memory
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* counts of prior Fibonacci residues modulo 6   */
    seq_t count[6] = {0};

    /* Fibonacci residues for s(k)=Fib(k+1)
       f_prev = s(k-1)   (initially s(1)=1)
       f_cur  = s(k)     (initially s(2)=2)      */
    seq_t f_prev = 1;
    seq_t f_cur  = 2;

    /* include s(1) in the counts for future j */
    count[(int)1] = 1;

    seq_t produced = 0;     /* number of sequence elements produced so far */

    for (idx_t k = 2; ; ++k)
    {
        seq_t r = f_cur;                  /* current residue s(k) mod 6 */
        seq_t c = count[(int)r];          /* how many j<k with same residue */

        if (c)
        {
            produced += c;
            if (produced >= n)
                return k;                 /* the n-th term */
        }

        /* make current k available for later j */
        count[(int)r] = c + 1;

        /* advance Fibonacci residues for the next k */
        seq_t f_next = f_prev + f_cur;
        f_next %= (seq_t)6;
        f_prev = f_cur;
        f_cur  = f_next;
    }
}
