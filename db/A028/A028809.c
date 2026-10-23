/*
 * A028809  Nonsquares mod 96.
 * https://oeis.org/A028809
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = 96*floor((n-1)/82) + B[(n-1)%82] where B =
 *            {2,3,5,6,7,8,10,11,12,13,14,15,17,18,19,20,21,22,23,24,
 *            26,27,28,29,30,31,32,34,35,37,38,39,40,41,42,43,44,45,46,
 *            47,50,51,52,53,54,55,56,58,59,60,61,62,63,65,66,67,68,69,
 *            70,71,72,74,75,76,77,78,79,80,82,83,84,85,86,87,88,89,90,
 *            91,92,93,94,95} i...
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const idx_t block_size   = 96;     /* size of each modulus block */
    const idx_t block_nonres = 82;     /* number of not‑quadratic residues per block */

    /* List of residues that are NOT quadratic residues modulo 96. */
    static const idx_t nonres_offsets[82] = {
        2, 3, 5, 6, 7, 8, 10, 11, 12, 13, 14, 15,
        17, 18, 19, 20, 21, 22, 23, 24,
        26, 27, 28, 29, 30, 31, 32,
        34, 35,
        37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47,
        50, 51, 52, 53, 54, 55, 56,
        58, 59, 60, 61, 62, 63,
        65, 66, 67, 68, 69, 70, 71, 72,
        74, 75, 76, 77, 78, 79, 80,
        82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95
    };

    idx_t block_index = (n - 1) / block_nonres;
    idx_t inner_index = (n - 1) % block_nonres;

    seq_t base = ((seq_t)block_index) * block_size;
    return base + nonres_offsets[inner_index];
}
