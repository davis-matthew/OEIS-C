/*
 * A232615  Variant of the Chandra-sutra (A014701) using 3 instead of 2, and a
 *          mod argument using residues 1 and 2.
 * https://oeis.org/A232615
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (# zeros in the base‑3 expansion of n) + 2 * ((# non‑zero
 *            digits of n) – 1) + (1 if the leading digit is 2, otherwise 0)
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t zeros   = 0;
    seq_t non_zero = 0;
    seq_t leading = 0;

    idx_t cur = n;
    while (cur != 0) {
        unsigned rem = cur % 3;
        cur /= 3;
        if (rem == 0) {
            zeros += 1;
        } else {
            non_zero += 1;
            leading = rem;   /* last non‑zero digit processed is the most significant */
        }
    }

    seq_t result = zeros;
    if (non_zero > 0) {
        result += 2 * (non_zero - 1);
    }
    if (leading == 2) {
        result += 1;
    }

    return result;
}
