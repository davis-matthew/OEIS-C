/*
 * A037762  Base 8 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 3,0,1,2.
 * https://oeis.org/A037762
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the number whose base‑8 digits are the first n terms of
 *            the periodic sequence 3,0,1,2,3,0,1,2,... a(n) =
 *            \sum_{k=0}^{n-1} d_k · 8^{\,n-1-k}, with d_k = [3,0,1,2][k mod
 *            4]
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A037762 */
seq_t a(idx_t n)
{
    /* Pattern of base‑8 digits: 3,0,1,2 repeated */
    static const uint8_t digits[4] = {3, 0, 1, 2};

    seq_t value = 0;
    for (idx_t i = 0; i < n; ++i) {
        /* use modulo to repeat the pattern */
        value = value * (seq_t)8 + (seq_t)digits[i & 3];
    }
    return value;
}
