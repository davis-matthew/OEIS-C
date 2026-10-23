/*
 * A191275  Numbers that are congruent to {0, 1, 3, 5, 7, 9, 11} mod 12.
 * https://oeis.org/A191275
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A191275: Numbers congruent to {0,1,3,5,7,9,11} mod 12
   Formula: a(n) = 12 * floor((n-1) / 7) + S[(n-1) % 7]
   with S = {0,1,3,5,7,9,11}.  Offset is 1 (a(1)=0). */
seq_t a(idx_t n)
{
    /* Handle n == 0 (outside offset) gracefully */
    if (n == 0) return 0;

    /* pattern of allowed residues per block of 12 */
    static const seq_t S[7] = {0, 1, 3, 5, 7, 9, 11};

    /* Compute block index k and position within block j */
    idx_t k = (n - 1) / 7;
    idx_t j = (n - 1) % 7;

    /* Return the value as 12*k + S[j] using seq_t arithmetic */
    return (seq_t)k * 12 + S[j];
}
