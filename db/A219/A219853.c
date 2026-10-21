/*
 * A219853  Number of 2 X n arrays of the minimum value of corresponding
 *          elements and their horizontal or antidiagonal neighbors in a
 *          random, but sorted with lexicographically nondecreasing rows and
 *          nonincreasing columns, 0..2 2 X n array.
 * https://oeis.org/A219853
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (9*n*n - 39*n + 82)/2 for n > 4 a(1) = 6, a(2) = 7, a(3)
 *            = 17, a(4) = 33
 * Range:     u8 n <= 7, u16 n <= 97, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    switch (n) {
    case 1: return 6;
    case 2: return 7;
    case 3: return 17;
    case 4: return 33;
    default:
        seq_t nn = (seq_t)n;
        /* compute (9*n*n - 39*n + 82)/2 exactly */
        seq_t num = ((seq_t)9 * nn * nn) - ((seq_t)39 * nn) + (seq_t)82;
        return num / 2;
    }
}
