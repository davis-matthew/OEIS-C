/*
 * A204990  (1/2)*(A204991).
 * https://oeis.org/A204990
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 36, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A204990: 
   For n odd:            a(n) = 2^{ord_2(n)} – 1          (ord_2(n) = multiplicative order of 2 modulo n)  
   For n even: let n = 2^{k}·m (m odd, k≥1) then 
   a(n) = 2^{k–1} · a(m)  

   The function below uses the helper big_pow(seq_t base, seq_t exp) 
   to compute powers of two in arbitrary-precision arithmetic. 
*/

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 1) return 1;

    /* Count powers of two in n */
    idx_t temp = n;
    idx_t v2   = 0;
    while ((temp & 1) == 0) {   /* temp is even */
        temp >>= 1;
        ++v2;
    }
    /* temp is now the odd part of n */
    if (v2 == 0) {   /* n is odd */
        /* Find multiplicative order of 2 modulo n */
        idx_t exp = 1;
        idx_t cur = 2 % n;
        while (cur != 1) {
            cur = (cur * 2) % n;
            ++exp;
        }
        seq_t pow2 = big_pow((seq_t)2, (seq_t)exp);
        return pow2 - 1;
    } else {   /* n is even */
        seq_t odd_val = a(temp);     /* recursive call on odd part */
        if (v2 == 1)                 /* 2^{0} * odd_val */
            return odd_val;
        seq_t mult = big_pow((seq_t)2, (seq_t)(v2 - 1));
        return mult * odd_val;
    }
}
