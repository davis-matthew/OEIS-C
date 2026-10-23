/*
 * A038212  Triangle whose (i,j)-th entry is binomial(i,j)*2^(i-j)*6^j.
 * https://oeis.org/A038212
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy, changed
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 24, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   binomial, pow2, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * a(n): row‑major enumeration of the triangle  
 *     T(i,j) = binomial(i,j)·2^(i‑j)·6^j (i,j ≥ 0, j ≤ i).
 *     The index n is converted to (i,j) via  
 *     i = ⌊(√(8n+1) – 1)/2⌋   (i.e. the largest i with i(i+1)/2 ≤ n)  
 *     j = n – i(i+1)/2.
 * Recurrence for the triangle:  T(n,k) = 6·T(n‑1,k‑1) + 2·T(n‑1,k),  
 * with T(0,0)=1 and T(n,k)=0 for k<0 or k>n.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"
#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    /* Find i such that i(i+1)/2 ≤ n < (i+1)(i+2)/2 */
    idx_t low = 0;
    idx_t high = n + 1;          /* high is exclusive */
    while (low + 1 < high) {
        idx_t mid = low + (high - low) / 2;
        __int128 tri = (__int128)mid * (mid + 1) / 2;
        if (tri <= n)
            low = mid;
        else
            high = mid;
    }
    idx_t i = low;
    idx_t tri_i = (i * (i + 1)) / 2;   /* i(i+1)/2 fits in idx_t */
    idx_t j = n - tri_i;

    /* Compute T(i,j) = binomial(i,j) * 2^(i-j) * 6^j */
    seq_t bin = binomial(i, j);
    seq_t pow2v = pow2(i - j);
    seq_t pow6v = pow_int((seq_t)6, j);

    return bin * pow2v * pow6v;
}
