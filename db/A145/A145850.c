/*
 * A145850  a(n) = A145818(2n-1).
 * https://oeis.org/A145850
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + 16 * s_{n-1}, where s_k is the k-th integer * whose
 *            base‑4 representation uses only digits 0 or 1. * Equivalently,
 *            s_k = interpret the binary digits of k as * base‑4 digits: s_k =
 *            Σ ((k >> i) & 1) * 4^i. * Thus a(n) = 1 + Σ_{i} ((n‑1) >> i & 1)
 *            * 2^{2i+4}
 * Range:     u8 n <= 4, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Since n ≥ 1 (offset 1), no underflow occurs. */
    idx_t k = n - 1;                 /* index in the base‑4‑digits only set */
    unsigned long long val64 = 1ULL; /* compute in 64‑bit to avoid UB */
    int i = 0;
    while (k) {
        if (k & 1ULL) {
            val64 += 1ULL << (2 * i + 4); /* 16 * 4^i = 2^(2i+4) */
        }
        k >>= 1;
        ++i;
    }
    return (seq_t)val64;
}
