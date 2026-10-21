/*
 * A096111  If n = 2^k - 1, then a(n) = k+1, otherwise a(n) =
 *          (A000523(n)+1)*a(A053645(n)).
 * https://oeis.org/A096111
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the product of the exponents in the binary expansion of
 *            2*n + 2
 * Range:     u8 n <= 58, u16 n <= 492, u32 n <= 8188, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (Equivalently, a(n) = ∏(i : bit i of (2n+2) is 1) i,  with i ≥ 1). */

seq_t a(idx_t n) {
    /* Compute m = 2*n + 2.  The shift is safe for the size of n considered. */
    idx_t m = (n << 1) + 2;

    seq_t result = (seq_t)1;
    idx_t pos = 0;              /* bit position, starting from 0 (least‑significant) */

    while (m) {
        if (m & 1) {
            /* Skip exponent 0 because 2n+2 is always even and never has bit 0 set. */
            if (pos > 0)
                result *= (seq_t)pos;
        }
        m >>= 1;
        pos++;
    }
    return result;
}
