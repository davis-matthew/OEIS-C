/*
 * A186079  Index of Fibonacci numbers having last digit less than
 *          corresponding tribonacci number.
 * https://oeis.org/A186079
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A186079: 
   a(n) = the n‑th positive integer k such that  
          Fib(k) mod 10 < Trib(k) mod 10 
   (Fib is Fibonacci with Fib(0)=0, Fib(1)=1;  
    Trib is tribonacci with Trib(1)=Trib(2)=Trib(3)=1) */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;  // undefined for n=0, but return 0

    idx_t k = 1;                /* current integer being tested */
    idx_t found = 0;            /* how many satisfying k's found */

    /* Fibonacci state: fib_prev = Fib(k-1), fib_cur = Fib(k) (mod 10) */
    int fib_prev = 0;           /* Fib(0) mod 10 */
    int fib_cur  = 1;           /* Fib(1) mod 10 */

    /* Tribonacci window: trib_window[0] = Trib(k-3), 
       trib_window[1] = Trib(k-2), trib_window[2] = Trib(k-1) (mod 10) */
    int trib_window[3] = {1, 1, 1}; /* Trib(1)=Trib(2)=Trib(3)=1 */

    for (;; ++k)
    {
        /* Fib(k) mod 10 */
        int fib_k;
        if (k == 1)                      /* Fib(1) */
            fib_k = fib_cur;
        else if (k == 2)                 /* Fib(2) */
            fib_k = fib_cur;
        else
            fib_k = fib_cur;             /* fib_cur already holds Fib(k) */

        /* Trib(k) mod 10 */
        int trib_k;
        if (k <= 3)
            trib_k = trib_window[k - 1];
        else {
            trib_k = (trib_window[0] + trib_window[1] + trib_window[2]) % 10;
            trib_window[0] = trib_window[1];
            trib_window[1] = trib_window[2];
            trib_window[2] = trib_k;
        }

        /* Test the condition */
        if (fib_k < trib_k) {
            ++found;
            if (found == n)
                return (seq_t)k;
        }

        /* Prepare Fibonacci state for next k */
        if (k == 1 || k == 2) {
            /* After computing Fib(k), update for next step */
            int next_fib = (fib_prev + fib_cur) % 10;   /* Fib(k+1) mod 10 */
            fib_prev = fib_cur;        /* Fib(k) becomes prev */
            fib_cur  = next_fib;       /* Fib(k+1) becomes cur */
        } else {
            /* For k >= 3, fib_cur currently holds Fib(k) */
            int next_fib = (fib_prev + fib_cur) % 10;   /* Fib(k+1) */
            fib_prev = fib_cur;        /* shift */
            fib_cur  = next_fib;
        }
    }
}
