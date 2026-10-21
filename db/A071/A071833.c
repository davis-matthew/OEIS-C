/*
 * A071833  Frequency ratios for notes of C-major scale starting at c = 24 and
 *          using Ptolemy's intense diatonic scale.
 * https://oeis.org/A071833
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy, nice
 * Formula:   a(n) = 2^⌊n/7⌋ * base[n mod 7], where base = {24, 27, 30, 32,
 *            36, 40, 45}
 * Range:     u8 n <= 23, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base values for the 7 notes of the C‑major scale.  */
    static const seq_t base[7] = {24, 27, 30, 32, 36, 40, 45};

    /* n mod 7 gives the position within the current octave,
       n / 7 gives how many octave doublings to apply.             */
    idx_t mod  = n % 7;
    idx_t oct  = n / 7;

    /* Compute 2^oct by repeated left shifting.  */
    seq_t result = base[mod];
    while (oct--) result <<= 1;   /* multiplication by 2 */

    return result;
}
