/*
 * A228587  Sum of the squares (modulo n) of the odd numbers less than n.
 * https://oeis.org/A228587
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{k odd < n} (k^2 mod n)
 * Range:     u8 n <= 33, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n <= 1) {
        return 0;
    }
    seq_t result = 0;
    for (idx_t k = 1; k < n; k += 2) {
        /* use 128-bit intermediate to avoid overflow on 64‑bit seq_t */
        seq_t kseq = (seq_t)k;
        seq_t mod = (seq_t)(((__int128)kseq * kseq) % (seq_t)n);
        result += mod;
    }
    return result;
}
