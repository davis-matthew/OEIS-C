/*
 * A162911  Numerators of drib tree fractions, where drib is the bit-reversal
 *          permutation tree of the Bird tree.
 * https://oeis.org/A162911
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, frac, nonn
 * Formula:   a(1) = 1; a(2n) = b(n); a(2n+1) = a(n) + b(n); b(1) = 1; b(2n) =
 *            a(n) + b(n); b(2n+1) = a(n); These recurrences are evaluated
 *            recursively to compute a(n)
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t get_a(idx_t n);
static seq_t get_b(idx_t n);

seq_t a(idx_t n) {
    return get_a(n);
}

static seq_t get_a(idx_t n) {
    if (n == 1)
        return (seq_t)1;
    if ((n & 1) == 0)          /* n = 2k  */
        return get_b(n >> 1);
    else                       /* n = 2k+1  */
        return get_a(n >> 1) + get_b(n >> 1);
}

static seq_t get_b(idx_t n) {
    if (n == 1)
        return (seq_t)1;
    if ((n & 1) == 0)          /* n = 2k  */
        return get_a(n >> 1) + get_b(n >> 1);
    else                       /* n = 2k+1  */
        return get_a(n >> 1);
}
