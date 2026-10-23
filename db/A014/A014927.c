/*
 * A014927  a(1)=1, a(n) = n*12^(n-1) + a(n-1).
 * https://oeis.org/A014927
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*12^(n-1) + a(n-1), a(1) = 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* safety for out‑of‑range input */
    seq_t value = 0;
    seq_t pow12 = 1;               /* 12^(k-1) for the current k */
    for (idx_t k = 1; k <= n; ++k) {
        value += (seq_t)k * pow12; /* add k * 12^(k-1) */
        pow12 *= (seq_t)12;        /* advance 12^(k) for next iteration */
    }
    return value;
}
