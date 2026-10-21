/*
 * A087410  Multiples of 8 with digits grouped in pairs and leading zeros
 *          omitted.
 * https://oeis.org/A087410
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = pair formed by concatenating digits of successive
 *            multiples of 8, grouping the concatenated digits into
 *            non‑overlapping pairs, and omitting leading zeros of each pair
 * Range:     u8 n <= 40, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: the function assumes that the index n is positive (n >= 1).  The
   concatenated digit stream is built from n*8 for n = 1, 2, 3, ….
   Pair number k uses digits at positions 2k−1 and 2k of this stream. */

seq_t a(idx_t n)
{
    /* position of the two digits that form the n-th pair */
    idx_t pos1 = 2 * n - 1;
    idx_t pos2 = 2 * n;
    idx_t maxPos = pos2;              /* maximum position needed */

    int  d1 = 0;                     /* first digit of the pair */
    int  d2 = 0;                     /* second digit of the pair */

    idx_t curPos = 1;                 /* current position in the stream 1‑based */
    idx_t mult = 1;                   /* current multiple of 8 */

    /* temporary arrays for digits of a number */
    int rev[20];
    int normal[20];

    /* Scan through successive multiples of 8 until both digits are found */
    while (curPos <= maxPos)
    {
        seq_t value = 8 * (seq_t)mult;   /* current multiple (fits in seq_t) */
        /* collect digits in reverse order */
        idx_t revLen = 0;
        do
        {
            rev[revLen++] = (int)(value % 10);
            value /= 10;
        } while (value > 0);

        /* reverse to get normal order */
        idx_t normLen = revLen;
        for (idx_t i = 0; i < normLen; ++i)
            normal[i] = rev[revLen - 1 - i];

        /* walk through the digits of the current multiple */
        for (idx_t i = 0; i < normLen; ++i)
        {
            if (curPos == pos1) d1 = normal[i];
            if (curPos == pos2) d2 = normal[i];

            ++curPos;
            if (curPos > maxPos) break;   /* both digits obtained */
        }

        ++mult;   /* move to the next multiple of 8 */
    }

    /* form the output value, omitting a leading zero in the pair */
    seq_t result = (seq_t)((d1 == 0) ? d2 : (d1 * 10 + d2));
    return result;
}
