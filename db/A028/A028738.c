/*
 * A028738  Nonsquares mod 25.
 * https://oeis.org/A028738
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = 25 * floor((n-1)/14) + r_{(n-1) mod 14}, where r_0 … r_13
 *            are the nonsquare residues modulo 25 sorted in ascending order:
 *            2,3,5,7,8,10,12,13,15,17,18,20,22,23
 * Range:     u8 n <= 14, u16 n <= 14, u32 n <= 14, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static int offsets[14];
    static int init = 0;
    if (!init) {
        unsigned char squares[25] = {0};
        for (idx_t x = 0; x < 25; ++x) {
            squares[(x * x) % 25] = 1;
        }
        int k = 0;
        for (int r = 0; r < 25; ++r) {
            if (!squares[r]) {
                offsets[k++] = r;
            }
        }
        init = 1;
    }
    idx_t q = (n - 1) / 14;        /* number of full 25‑periods preceding */
    idx_t r = (n - 1) % 14;        /* position inside the current period */
    seq_t result = (seq_t)(25 * q) + (seq_t)offsets[r];
    return result;
}
