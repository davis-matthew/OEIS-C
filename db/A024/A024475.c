/*
 * A024475  s(1)t(n) + s(2)t(n-1) + ... + s(k)t(n+1-k), where k = [ (n+1)/2 ],
 *          s = (Lucas numbers), t = A001950 (upper Wythoff sequence).
 * https://oeis.org/A024475
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{i=1}^{⌊(n+1)/2⌋} L_i * (⌊(n+1-i)·φ⌋ + (n+1-i)) where
 *            L_i are Lucas numbers (L_0=2, L_1=1) and φ = (1+√5)/2
 * Range:     u8 n <= 8, u16 n <= 26, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/*  φ ≈ 1.6180339887498948482045868343656381  (long double precision)  */
#define PHI_L ((long double)1.6180339887498948482045868343656381L)

seq_t a(idx_t n) {
    /* number of terms in the convolution */
    idx_t k = (n + 1) / 2;

    seq_t result = 0;

    /* lucas numbers L0 = 2, L1 = 1 */
    seq_t prev = (seq_t)2;   /* L0 */
    seq_t curr = (seq_t)1;   /* L1 */

    for (idx_t i = 1; i <= k; ++i) {
        /* current lucas number L_i */
        seq_t lucas = curr;

        /* index for the Wythoff element */
        idx_t j = n + 1 - i;

        /* floor(j * φ) computed with long double */
        long double prod = (long double)j * PHI_L;
        uint64_t floor_val = (uint64_t)prod;             /* truncation gives floor */

        /* upper Wythoff t(j) = floor(j*φ) + j */
        seq_t t_val = (seq_t)(floor_val + j);

        result += lucas * t_val;

        /* next lucas number L_{i+1} */
        seq_t next = prev + curr;
        prev = curr;
        curr = next;
    }

    return result;
}
