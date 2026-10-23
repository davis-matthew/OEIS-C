/*
 * A187875  Numbers from zero to ten, sorted alphabetically by their French
 *          name.
 * https://oeis.org/A187875
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, easy, word, full
 * Formula:   a(n) = v[(n-1) mod 11] where v = {5, 2,10, 8, 9, 4, 7, 6, 3, 1,
 *            0} (French alphabetical order of 0–10)
 * Range:     u8 n <= 11, u16 n <= 11, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The 11 numbers (0–10) sorted alphabetically by French name. */
    static const seq_t order[11] = {
        (seq_t)5, (seq_t)2, (seq_t)10, (seq_t)8, (seq_t)9,
        (seq_t)4, (seq_t)7, (seq_t)6, (seq_t)3, (seq_t)1, (seq_t)0
    };

    /* n is 1‑based; (n-1) is safe because n ≥ 1. */
    idx_t idx = (n - 1) % 11;
    return order[idx];
}
