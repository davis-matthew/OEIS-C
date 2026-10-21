/*
 * A056092  Numbers k such that k^6 == 1 (mod 7^4).
 * https://oeis.org/A056092
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2401*floor((n-1)/6) + residues[(n-1)%6] where residues =
 *            {1,1047,1048,1353,1354,2400}
 * Range:     u8 n <= 1, u16 n <= 163, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    static const seq_t residues[6] = {
        (seq_t)1,   (seq_t)1047, (seq_t)1048,
        (seq_t)1353, (seq_t)1354, (seq_t)2400
    };
    idx_t block = (n - 1) / 6;
    seq_t res   = residues[(n - 1) % 6];
    return (seq_t)block * (seq_t)2401 + res;
}
