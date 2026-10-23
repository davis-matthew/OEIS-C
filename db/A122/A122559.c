/*
 * A122559  Position of first letter of n-th planet (in English) in alphabet.
 * https://oeis.org/A122559
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word
 * Formula:   a(n) = position of the first letter of the English name of the
 *            n-th planet in the alphabet (A=1,...,Z=26), 0 for n<1 or n>8
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const char first[8] = {'M','V','E','M','J','S','U','N'};
    if (n < 1 || n > 8) return 0;
    idx_t i = n - 1;
    return (seq_t)(first[i] - 'A' + 1);
}
