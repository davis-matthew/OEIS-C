/*
 * A086409  a(1)=1; a(n+1)=a(n)-1 if (prime(a(n))+1)/2 is even, else
 *          a(n+1)=a(n)+4.
 * https://oeis.org/A086409
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
Recurrence: a(1)=1; for n≥1 let p=prime(a(n)).  
If ((p+1)/2 is even) a(n+1)=a(n)-1; otherwise a(n+1)=a(n)+4.  
The sequence enters the cycle 46,45,49,48,47 starting at a(26)=46. 
*/

#include <stddef.h>

seq_t a(idx_t n)
{
    /* First 25 terms computed directly */
    static const seq_t init[25] = {
        1, 5, 4, 3, 7, 11, 10, 14, 13, 17,
        16, 20, 19, 18, 22, 21, 25, 29, 33, 37,
        41, 40, 44, 48, 47
    };

    if (n <= 25) {
        return init[n - 1];
    }

    /* Cycle: 46,45,49,48,47 repeats for n ≥ 26 */
    static const seq_t cycle[5] = {46, 45, 49, 48, 47};
    size_t idx = (size_t)((n - 26) % 5);
    return cycle[idx];
}
