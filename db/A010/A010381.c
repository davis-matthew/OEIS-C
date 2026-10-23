/*
 * A010381  Squares mod 19.
 * https://oeis.org/A010381
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = the (n-1)th smallest quadratic residue modulo 19 (sorted
 *            list 0,1,4,5,6,7,9,11,16,17) repeated with period 10
 * Range:     u8 n <= 10, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Sorted distinct residues of i² mod 19. */
    static const seq_t residues[10] = {
        (seq_t)0, (seq_t)1, (seq_t)4, (seq_t)5, (seq_t)6,
        (seq_t)7, (seq_t)9, (seq_t)11, (seq_t)16, (seq_t)17
    };

    /* n ≥ 1, so n-1 is safe. Reduce modulo 10. */
    idx_t idx_mod = (n - (idx_t)1) % (idx_t)10;
    return residues[(size_t)idx_mod];
}
