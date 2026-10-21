/*
 * A075567  Rearrangement of natural numbers by choosing alternately odd and
 *          even numbers such that every partial sum is composite.
 * https://oeis.org/A075567
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n for n>9; the first nine terms are specially arranged
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* First nine terms as given in the OEIS entry */
    static const seq_t first9[9] = {9, 6, 1, 2, 3, 4, 5, 8, 7};

    if (n <= 9)
        return first9[n - 1];

    /* For all n > 9, the sequence equals the index */
    return (seq_t)n;
}
