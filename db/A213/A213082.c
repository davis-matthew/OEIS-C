/*
 * A213082  Values of n for which the number of roots of the function sin(x)/x
 *          - 1/n increases.
 * https://oeis.org/A213082
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 42, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   count_sevens
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A213082: a(n) = 2 + 6·(n−2) + c(n)   (n ≥ 3)  
   where c(n) counts how many times the difference 7 occurs up to n.
   The 7‑differences occur at indices n = 4, 8, 11, 15, 18, 22, 25, 29, 32, …  
   These indices satisfy:  
       e₀ = 4  
       e_{2m}   = 4 + 7·m   (even m)  
       e_{2m+1} = 8 + 7·m   (odd m).          */
#include <stdint.h>
#include "../utils/count_sevens.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;               /* undefined, but safe for idx_t ≥ 1 */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)8;               /* 2 + 6·1 + 0 */

    if (n < 4) return (seq_t)8;                /* safety for n == 3 */

    idx_t cnt = count_sevens(n);
    seq_t result = (seq_t)2 + (seq_t)6 * (seq_t)(n - 2) + (seq_t)cnt;
    return result;
}
