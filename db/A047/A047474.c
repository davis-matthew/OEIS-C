/*
 * A047474  Numbers that are congruent to {0, 2, 3} mod 8.
 * https://oeis.org/A047474
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*k - c where k and c depend on n mod 3: * if n ≡ 0 (mod
 *            3): k = n/3, c = 5 * if n ≡ 1 (mod 3): k = (n+2)/3, c = 8 * if n
 *            ≡ 2 (mod 3): k = (n+1)/3, c = 6 (derived from a(3k)=8k-5,
 *            a(3k-1)=8k-6, a(3k-2)=8k-8)
 * Range:     u8 n <= 96, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* offset 1: a(1)=0, etc. */
    idx_t r = n % 3;
    if (r == 0) {
        idx_t k = n / 3;
        return (seq_t)k * 8 - 5;
    } else if (r == 1) {
        idx_t k = (n + 2) / 3;
        return (seq_t)k * 8 - 8;
    } else { /* r == 2 */
        idx_t k = (n + 1) / 3;
        return (seq_t)k * 8 - 6;
    }
}
