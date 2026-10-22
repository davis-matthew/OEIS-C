/*
 * A002617  Weight distribution of Cheng-Sloane [ 32,17,8 ] code.
 * https://oeis.org/A002617
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   Weight distribution of [32, 17, 8] Cheng-Sloane code; symmetric:
 *            a(n) = a(32-n)
 * Range:     u8 n <= 3, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n > 32) return 0;
    
    /* Use symmetry: a(n) = a(32-n) */
    if (n > 16) n = 32 - n;
    
    static const seq_t weights[17] = {
        1, 0, 0, 0, 908, 3328, 14784, 27392, 38246, 27392, 14784, 3328, 908, 0, 0, 0, 1
    };
    
    return weights[n];
}
