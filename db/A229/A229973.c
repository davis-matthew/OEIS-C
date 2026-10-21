/*
 * A229973  Numbers coprime to 39.
 * https://oeis.org/A229973
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 39 * ((n-1)/24) + res[(n-1) % 24] where res is the list
 *            of the 24 integers in [1,39] coprime to 39
 * Range:     u8 n <= 157, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const seq_t residues[24] = {
    1, 2, 4, 5, 7, 8, 10, 11,
    14, 16, 17, 19, 20, 22, 23, 25,
    28, 29, 31, 32, 34, 35, 37, 38
};

seq_t a(idx_t n)
{
    /* We assume n >= 1; behavior for n < 1 is undefined. */
    idx_t k = (n - 1) / 24;          /* how many complete 39‑blocks */
    idx_t r = (n - 1) % 24;          /* index inside the current block */
    seq_t base = (seq_t)39 * (seq_t)k;
    return base + residues[r];
}
