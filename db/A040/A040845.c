/*
 * A040845  Continued fraction for sqrt(875).
 * https://oeis.org/A040845
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A040845: Continued fraction for sqrt(875)
   a(0) = 29
   For n ≥ 1, a(n) repeats a 16‑term period
   [1, 1, 2, 1, 1, 1, 1, 3, 1, 1, 1, 1, 2, 1, 1, 58]        */
seq_t a(idx_t n)
{
    /* Periodic part of the continued fraction (length 16). */
    static const seq_t period[16] = {
        (seq_t)1, (seq_t)1, (seq_t)2, (seq_t)1,
        (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)3,
        (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)1,
        (seq_t)2, (seq_t)1, (seq_t)1, (seq_t)58
    };

    if (n == 0) {
        return (seq_t)29;
    }

    /* a(n) = period[(n-1) mod 16] for n ≥ 1 */
    idx_t idx = (n - 1) & 15;   /* modulo 16 via bitmask */
    return period[idx];
}
