/*
 * A225858  Numbers of the form 2^i*3^j*(12k+7) or 2^i*3^j*(12k+11), i, j, k
 *          >= 0.
 * https://oeis.org/A225858
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 113, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence of numbers whose odd part after removing all factors of 2
   and 3 is congruent to 7 or 11 modulo 12 (OEIS A225858).  */
/* a(1) = 7, a(2) = 11, a(3) = 14, ... */

seq_t a(idx_t n)
{
    idx_t   count = 0;   /* how many terms have been found so far */
    seq_t   num   = 1;   /* current integer being tested */

    while (1) {
        seq_t odd = num;

        /* strip factors of 2 */
        while ((odd % 2) == 0)
            odd /= 2;

        /* strip factors of 3 */
        while ((odd % 3) == 0)
            odd /= 3;

        /* check remainder modulo 12 */
        if ((odd % 12) == 7 || (odd % 12) == 11) {
            ++count;
            if (count == n)
                return num;
        }
        ++num;
    }
}
