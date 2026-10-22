/*
 * A003509  a(n) is the index of the first occurrence of n in A155934(m) =
 *          k(m) = the least integer such that every m X m (0,1)-matrix with
 *          exactly k(m) ones in each row and in each column contains a 2 X 2
 *          submatrix without zeros.
 * https://oeis.org/A003509
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = (n-1)*(n-2) + 1 for n >= 3, with a(2) = 2
 * Range:     u8 n <= 7, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 2) {
        return (seq_t)2;
    }
    
    seq_t nm1 = (seq_t)(n - 1);
    seq_t nm2 = (seq_t)(n - 2);
    return nm1 * nm2 + (seq_t)1;
}
