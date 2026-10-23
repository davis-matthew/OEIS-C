/*
 * A023498  a(n) = b(n) + d(n), where b(n) = (n-th Lucas number > 3) and d(n)
 *          = (n-th number that is 1 or 2 or is not a Fibonacci number).
 * https://oeis.org/A023498
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 20, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS Sequence A023498
    a(n) = b(n) + d(n)
    b(n) = (n+3)-rd Lucas number
    d(n) = n-th number that is 1 or 2 or not a Fibonacci number
    offset: 0  (a(0) = 5)
*/

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* --- compute b(n) : (n+3)-rd Lucas number -------------------------------- */
    seq_t L0 = 2;          /* L0 = 2  */
    seq_t L1 = 1;          /* L1 = 1  */
    seq_t Lnext;
    idx_t limit = n + 3;   /* we need L_{n+3} */

    if (limit >= 2) {
        idx_t i = 2;
        while (i <= limit) {
            Lnext = L0 + L1; /* L_k = L_{k-1} + L_{k-2} */
            L0 = L1;
            L1 = Lnext;
            ++i;
        }
    }
    seq_t b = L1;          /* b(n) = Lucas(n+3) */

    /* --- compute d(n) : n-th number that is 1 or 2 or not Fibonacci ------------ */
    idx_t   count = 0;          /* how many qualifying numbers found so far          */
    uint64_t m    = 1;          /* candidate integer to test                       */
    uint64_t f_prev = 1, f_curr = 1; /* to generate Fibonacci numbers on the fly       */
    seq_t d = 0;                /* value of d(n) once found                         */

    while (true) {
        bool include = false;

        if (m == 1 || m == 2) {
            include = true;          /* 1 and 2 are always included */
        } else {
            /* advance Fibonacci generation until f_curr >= m   */
            while (f_curr < m) {
                uint64_t f_next = f_prev + f_curr;
                f_prev = f_curr;
                f_curr = f_next;
            }

            if (f_curr == m)   /* m is a Fibonacci number > 2, exclude */
                include = false;
            else
                include = true;  /* m is not Fibonacci, include          */
        }

        if (include) {
            if (count == n) {
                d = (seq_t)m;   /* found the n-th qualifying number      */
                break;
            }
            ++count;
        }
        ++m;
    }

    return b + d;
}
