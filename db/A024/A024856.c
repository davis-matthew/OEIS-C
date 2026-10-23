/*
 * A024856  a(n) = s(1)t(n) + s(2)t(n-1) + ... + s(k)t(n-k+1), where k = [ n/2
 *          ], s = (natural numbers), t = A023532.
 * https://oeis.org/A024856
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 47, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A024856: a(n) = Σ_{i=1}^{⌊n/2⌋} i * t(n-i+1)   (t = A023532)
   It can be rewritten as
     a(n) = k(k+1)/2 - Σ_{x ∈ Z , n-k+1 ≤ x ≤ n} (n-x+1)
   where k = ⌊n/2⌋ and  
   Z = { (m^2+5m+4)/2 : m ≥ 0 }  (indices where t(x)=0). 
   The series Z is 2,5,9,14,20,27,35,…  (differences increase by 1).
   This implementation maintains the list of zeros up to the requested n
   and computes the sum over the relevant window in O(√n) time.
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    /*  Compute k = floor(n/2).  */
    idx_t k = n >> 1;                      /* integer division by 2 */

    /*  Keep a static list of indices where t(x)=0.  */
    static idx_t *zero = NULL;
    static size_t zero_cap = 0;
    static size_t zero_cnt = 0;
    static idx_t last_zero = 0;            /* last element stored */
    static idx_t delta = 0;                /* next increment to add */
    static int initialized = 0;

    /*  Ensure the zero list contains all values ≤ n.  */
    if (!initialized) {
        if (n >= 2) {
            /* initial capacity */
            zero_cap = 16;
            zero = (idx_t *)malloc(zero_cap * sizeof(idx_t));
            zero_cnt = 0;
            last_zero = 2;                 /* first zero is 2 */
            delta = 3;                     /* next increment */
            zero[zero_cnt++] = last_zero;  /* store 2 */
            initialized = 1;
        }
        else {
            /* n < 2: no zeros, nothing to do */
            initialized = 1;
        }
    }

    /*  Grow the zero list up to n.  */
    while (initialized && last_zero + delta <= n) {
        last_zero += delta;
        delta++;
        if (zero_cnt >= zero_cap) {
            zero_cap <<= 1;
            zero = (idx_t *)realloc(zero, zero_cap * sizeof(idx_t));
        }
        zero[zero_cnt++] = last_zero;
    }

    /*  Compute triangular number k(k+1)/2 in seq_t.  */
    seq_t triangular = (seq_t)k * (k + 1) / 2;

    /*  Subtract contributions from zeros in the window [n-k+1 , n].  */
    idx_t start = n - k + 1;
    seq_t subtract = 0;
    for (size_t i = 0; i < zero_cnt; ++i) {
        idx_t x = zero[i];
        if (x < start) continue;
        if (x > n)  break;
        subtract += (seq_t)(n - x + 1);
    }

    return triangular - subtract;
}
