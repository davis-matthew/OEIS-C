/*
 * A028756  Nonsquares mod 43.
 * https://oeis.org/A028756
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 21, u32 n <= 21, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition: a(n) = the nth positive integer whose residue modulo 43 is not a quadratic residue (i.e., not a square modulo 43). */

seq_t a(idx_t n)
{
    /* squares modulo 43: index is residue, value is 1 if a square, 0 otherwise */
    static const unsigned char is_square[43] = {
        1, /* 0   */
        1, /* 1   */
        0, /* 2   */
        0, /* 3   */
        1, /* 4   */
        0, /* 5   */
        1, /* 6   */
        0, /* 7   */
        0, /* 8   */
        1, /* 9   */
        1, /* 10  */
        1, /* 11  */
        0, /* 12  */
        1, /* 13  */
        1, /* 14  */
        1, /* 15  */
        1, /* 16  */
        1, /* 17  */
        0, /* 18  */
        0, /* 19  */
        0, /* 20  */
        1, /* 21  */
        0, /* 22  */
        1, /* 23  */
        1, /* 24  */
        1, /* 25  */
        0, /* 26  */
        0, /* 27  */
        0, /* 28  */
        0, /* 29  */
        0, /* 30  */
        1, /* 31  */
        0, /* 32  */
        0, /* 33  */
        0, /* 34  */
        1, /* 35  */
        1, /* 36  */
        0, /* 37  */
        1, /* 38  */
        0, /* 39  */
        1, /* 40  */
        1, /* 41  */
        0  /* 42  */
    };

    seq_t candidate = 0;
    idx_t found = 0;

    /* Count until the nth nonsquare is found */
    while (found < n)
    {
        candidate++;
        idx_t residue = (idx_t)(candidate % (seq_t)43);
        if (!is_square[residue])
        {
            found++;
            if (found == n)
                return candidate;
        }
    }
    /* Should never reach here */
    return 0;
}
