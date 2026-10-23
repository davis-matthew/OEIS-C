/*
 * A132739  Largest divisor of n not divisible by 5.
 * https://oeis.org/A132739
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = largest divisor of n not divisible by 5
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Convert index to sequence type to avoid intermediate 64‑bit overflow
       and perform division until the result is not divisible by 5. */
    seq_t res = (seq_t)n;
    while (res % 5 == 0) {
        res /= 5;
    }
    return res;
}
