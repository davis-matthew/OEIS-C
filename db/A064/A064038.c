/*
 * A064038  Numerator of average number of swaps needed to bubble sort a
 *          string of n distinct letters.
 * https://oeis.org/A064038
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, frac
 * Formula:   a(n) = numerator of n*(n-1)/4, i.e. if n≡0 or 1 (mod 4): a(n) =
 *            n*(n-1)/4 if n≡2 or 3 (mod 4): a(n) = n*(n-1)/2
 * Range:     u8 n <= 25, u16 n <= 362, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Reduce before multiplication to avoid overflow */
    idx_t a_part = n;
    idx_t b_part = n - 1;

    /* Determine denominator type by n mod 4 */
    if ((a_part & 3) == 0 || (a_part & 3) == 1) {
        /* Denominator 1 : divide by 4 */
        /* Remove two factors of 2 */
        if (a_part & 1) {          /* a_part odd, b_part even */
            b_part /= 2;
            b_part /= 2;
        } else {                   /* a_part even */
            a_part /= 2;
            a_part /= 2;
        }
    } else {
        /* Denominator 2 : divide by 2 once */
        if (a_part & 1)            /* a_part odd */
            b_part /= 2;
        else
            a_part /= 2;
    }

    /* Multiply using a width that can hold the intermediate result */
    unsigned __int128 prod = (unsigned __int128)a_part * b_part;
    return (seq_t)prod;
}
