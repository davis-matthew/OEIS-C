/*
 * A007664  Reve's puzzle: number of moves needed to solve the Towers of Hanoi
 *          puzzle with 4 pegs and n disks, according to the Frame-Stewart
 *          algorithm.
 * https://oeis.org/A007664
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = 1 + (n - 1 - A003056(n)*(A003056(n) - 1)/2)*2^A003056(n)
 *            where A003056(n) = largest k such that k*(k+1)/2 <= n
 * Range:     u8 n <= 18, u16 n <= 82, u32 n <= 383, u64 n <= 1717,
 *            u128 n <= 7388, big any n
 * Helpers:   A001832_pow2, a000037_isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/A001832_pow2.h"
#include "../utils/a000037_isqrt.h"


static idx_t A003056(idx_t n) {
    uint64_t disc = 1 + 8 * n;
    uint64_t sqrt_disc = a000037_isqrt(disc);
    return (sqrt_disc - 1) / 2;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    
    idx_t k = A003056(n);
    seq_t pow2_k = A001832_pow2(k);
    seq_t tri = ((seq_t)k * ((seq_t)k - 1)) / 2;
    seq_t coeff = (seq_t)n - 1 - tri;
    
    return 1 + coeff * pow2_k;
}
