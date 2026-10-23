/*
 * A180313  A sequence a(n) such that a(n+1)^2 - a(n)^2 are perfect squares.
 * https://oeis.org/A180313
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 3 a(n) = 5 * 17^{(n/2)-1} if n is even (n ≥ 2) a(n) = 13
 *            * 17^{(n-3)/2} if n is odd (n ≥ 3) (The exponent is the integer
 *            power of 17; the sequences for even and odd indices differ only
 *            by a multiplier of 5 or 13.)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 16, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_seq.h"

/* Return the nth term of OEIS A180313.
   The build supplies the types seq_t (unsigned integer or arbitrary
   precision) and idx_t (unsigned 64‑bit).  The function may be called
   in any order; it uses only the pow_seq routine from the utilities. */
seq_t a(idx_t n) {
    if (n == 1)
        return (seq_t)3;

    if (n & 1) {                 /* odd index (≥3) */
        idx_t k = (n - 1) / 2;   /* k ≥ 1, same as ⌊(n-1)/2⌋ */
        seq_t p = pow_seq((seq_t)17, (int)(k - 1));
        return (seq_t)13 * p;
    } else {                    /* even index (≥2) */
        idx_t k = n / 2;         /* k ≥ 1 */
        seq_t p = pow_seq((seq_t)17, (int)(k - 1));
        return (seq_t)5 * p;
    }
}
