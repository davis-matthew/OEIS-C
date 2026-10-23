/*
 * A102678  Number of digits >= 6 in the decimal representations of all
 *          integers from 0 to n.
 * https://oeis.org/A102678
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 338, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A102678: Number of digits >= 6 in the decimal representations of all
 * integers from 0 to n.
 *
 * Formula (digit–dp style):
 *   a(n) = Σ_{j≥0} [ 4·10ᶦ · ⌊n / 10⁽ᶦ⁺¹⁾⌋
 *                    + extra(j) ],
 *   where for the j‑th digit (10ʲ place):
 *     let high  = ⌊n / 10ʲ⁺¹⌋,
 *         cur   = ⌊n / 10ʲ⌋ mod 10,
 *         low   = n mod 10ʲ.
 *     If cur < 6:   extra(j) = 0.
 *     else:         extra(j) = (cur−6)·10ʲ + (low+1).
 *
 * The implementation below follows this formula exactly using only integer
 * arithmetic.  It is safe for any idx_t that fits into 64 bits and will
 * produce correct results as long as the intermediate and final values
 * fit into the seq_t type chosen by the build.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t total = 0;
    idx_t n_u = n;          /* local copy of the index as idx_t       */
    idx_t p = 1;            /* 10^j represented as idx_t (fits 64‑bit) */

    while (p <= n_u) {
        /* high = ⌊n / (p*10)⌋ ; avoid overflow by checking limits   */
        idx_t high = (p > n_u / 10) ? 0 : n_u / (p * 10);
        idx_t cur   = (n_u / p) % 10;
        idx_t low   = n_u % p;

        /* Convert to seq_t for the accumulation   */
        seq_t high_seq = (seq_t)high;
        seq_t p_seq    = (seq_t)p;

        seq_t contrib = high_seq * 4 * p_seq;   /* 4 * 10^j * high */

        if (cur >= 6) {
            seq_t less = (seq_t)(cur - 6);       /* digits 6..cur-1 */
            contrib += less * p_seq;             /* (cur-6) * 10^j */
            contrib += (seq_t)(low + 1);         /* + (low+1)   */
        }

        total += contrib;

        /* Advance to next power of 10, avoiding overflow */
        if (p > n_u / 10) break;   /* next p would overflow or exceed n */
        p *= 10;
    }

    return total;
}
