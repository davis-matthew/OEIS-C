/*
 * A038809  a(n) is the number of ways to write n in bases 2-10 such that the
 *          digit k-1 appears in the representation in base k.
 * https://oeis.org/A038809
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of bases k (2 ≤ k ≤ 10) such that the digit k-1
 *            appears in the base‑k representation of n
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 0;
    for (idx_t k = 2; k <= 10; ++k) {
        seq_t tmp = (seq_t)n;
        seq_t target = (seq_t)(k - 1);
        while (tmp > 0) {
            seq_t digit = tmp % (seq_t)k;
            if (digit == target) {
                ++result;
                break;
            }
            tmp /= (seq_t)k;
        }
    }
    return result;
}
