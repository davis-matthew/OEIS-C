/*
 * A070922  First palindromic prime pyramids with a(1)=2 such that (number of
 *          digits in a(n+1)) = (number of digits in a(n)) + 2.
 * https://oeis.org/A070922
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, fini, full, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A070922 - first palindromic prime pyramids with a(1)=2,
   digits increasing by 2.  There are only 5 terms, after which the
   sequence has no more values.  Return 0 for n > 5. */
seq_t a(idx_t n)
{
    /* The 5 terms of the pyramid (all fit within 64 bits). */
    static const seq_t seq[5] = {
        2ULL,
        929ULL,
        39293ULL,
        7392937ULL,
        373929373ULL
    };

    /* idx_t is unsigned; the function is called only for n >= 1.
       If n is within the known range, return the term; otherwise return 0. */
    if (n >= 1 && n <= 5)
        return seq[n - 1];
    return 0;
}
