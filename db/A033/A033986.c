/*
 * A033986  Order of first letter of n-th month of the year.
 * https://oeis.org/A033986
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, less, nonn, word
 * Formula:   a(n) = alphabet position (A=1, B=2, …, Z=26) of the first letter
 *            of the n‑th month of the year
 * Range:     u8 n <= 12, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const char first_letters[12] = {
        'J', 'F', 'M', 'A', 'M', 'J', 'J', 'A',
        'S', 'O', 'N', 'D'
    };
    return (seq_t)(first_letters[(n - 1) % 12] - 'A' + 1);
}
