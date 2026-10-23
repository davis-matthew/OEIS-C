/*
 * A126648  A 3 X 3 magic square with magic sum 123 and entries congruent to 1
 *          (mod 10): equals 10*A033812 - 9.
 * https://oeis.org/A126648
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, tabf
 * Formula:   a(n) = 10*A033812(n) - 9
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* The 3×3 normal Lo‑Shu magic square (row‑major order) */
static const seq_t lo_shu[9] = {8, 1, 6, 3, 5, 7, 4, 9, 2};

seq_t a(idx_t n)
{
    /* n−1 modulo 9 selects the correct element of the base square */
    idx_t idx = (n - 1) % 9;
    /* Apply the given transformation */
    return (seq_t)10 * lo_shu[idx] - (seq_t)9;
}
