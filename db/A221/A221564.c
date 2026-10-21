/*
 * A221564  The number of FO4C3 moves required to restore a packet of n
 *          playing cards to its original state (order and orientation), where
 *          each move Flips Over the top four (4) as a unit and then Cuts
 *          three (3) cards from the top to bottom.
 * https://oeis.org/A221564
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   * a(3*k) = 2*k*(k-1) * a(3*k+1) = 2*k * a(3*k+2) = 2*k*(k+1) *
 *            for k ≥ 1 (a(4)=2, a(5)=4, a(6)=4, …) * The function implements
 *            this exact formula in O(1) time
 * Range:     u8 n <= 34, u16 n <= 544, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 4) return 0;

    idx_t mod = n % 3;
    seq_t result;

    if (mod == 0) {
        idx_t k = n / 3;
        seq_t kk = (seq_t)k;
        result = 2 * kk * (kk - 1);
    } else if (mod == 1) {
        idx_t k = (n - 1) / 3;
        result = 2 * (seq_t)k;
    } else { /* mod == 2 */
        idx_t k = (n - 2) / 3;
        seq_t kk = (seq_t)k;
        result = 2 * kk * (kk + 1);
    }

    return result;
}
