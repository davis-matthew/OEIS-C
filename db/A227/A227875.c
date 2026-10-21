/*
 * A227875  Fibonacci numbers which are perfect powers.
 * https://oeis.org/A227875
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, bref, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A227875: Fibonacci numbers that are perfect powers.  
   The only terms are 0, 1, 8, and 144. For n > 4 the sequence is defined
   to be 0 (the OEIS convention for finite sequences). 
*/
#include <stdbool.h>

seq_t a(idx_t n)
{
    switch (n) {
        case 1: return (seq_t)0;   /* F(0) = 0 = 0^k */
        case 2: return (seq_t)1;   /* F(1) = 1 = 1^k */
        case 3: return (seq_t)8;   /* F(6) = 8 = 2^3 */
        case 4: return (seq_t)144; /* F(12) = 144 = 12^2 */
        default: return (seq_t)0;  /* no further Fibonacci perfect powers */
    }
}
