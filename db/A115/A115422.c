/*
 * A115422  Numbers k > 0 such that k XOR 20*k = 21*k.
 * https://oeis.org/A115422
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 783, u32 n <= 9231, u64 n <= 9231,
 *            u128 n <= 9231, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) is the n‑th positive integer k such that (k & (20*k)) == 0 */
seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t k = 1;
    for (;; ++k) {
        unsigned __int128 prod = (unsigned __int128)k * 20;
        if (((unsigned __int128)k & prod) == 0) {
            ++count;
            if (count == n)
                return (seq_t)k;
        }
    }
}
