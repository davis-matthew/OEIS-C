/*
 * A137482  Number of permutations of n objects such that no two-element
 *          subset is preserved.
 * https://oeis.org/A137482
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = (n-1)*a(n-1) - a(n-2) + (n-2)*n*a(n-3)   (n > 2)
 * Initial values: a(0) = 1, a(1) = 1, a(2) = 0
 */
 
#include <stdint.h>
 
seq_t a(idx_t n)
{
    /* For n < 3 return the initial values directly */
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)1;
    if (n == 2u) return (seq_t)0;
 
    /* a(n-3), a(n-2), a(n-1) for the recurrence */
    seq_t a_nm3 = (seq_t)1;   /* a(0) */
    seq_t a_nm2 = (seq_t)1;   /* a(1) */
    seq_t a_nm1 = (seq_t)0;   /* a(2) */
 
    for (idx_t i = 3u; i <= n; ++i) {
        /* term = (i-1)*a_nm1 - a_nm2 + (i-2)*i*a_nm3 */
        seq_t term = ((seq_t)(i - 1u)) * a_nm1
                   - a_nm2
                   + ((seq_t)(i - 2u)) * ((seq_t)i) * a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = term;
    }
    return a_nm1;
}
