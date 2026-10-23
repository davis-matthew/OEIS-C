/*
 * A198681  Nonnegative multiples of 3 whose sum of base-3 digits are of the
 *          form 3k+1.
 * https://oeis.org/A198681
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 1001, u32 n <= 1001, u64 n <= 1001,
 *            u128 n <= 1001, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A198681. Compute the nth (1‑based) term of nonnegative multiples of 3
 * whose sum of base‑3 digits is congruent to 1 modulo 3.
 * The function scans successive multiples of 3 and tests each until the nth term
 * is found. */
static seq_t digit_sum_base3(seq_t x) {
    seq_t sum = 0;
    while (x != 0) {
        sum += x % (seq_t)3;
        x /= (seq_t)3;
    }
    return sum;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;  /* offset irrelevant */
    idx_t seen = 0;
    seq_t val = 0;
    for (idx_t k = 0;; ++k) {
        val = (seq_t)3 * (seq_t)k;
        if (digit_sum_base3(val) % (seq_t)3 == (seq_t)1) {
            ++seen;
            if (seen == n) return val;
        }
    }
}
