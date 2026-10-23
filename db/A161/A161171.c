/*
 * A161171  Erroneous version of A035008.
 * https://oeis.org/A161171
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   not stated
 * Range:     i8 n <= 4, i16 n <= 15, i32 n <= 15, i64 n <= 15, i128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 8 (n-4)^2 + (n-2)*16 + 46 + (n-2)*24   for n > 4
 *   a(1) = 0
 *   a(2) = 0
 *   a(3) = 12
 *   a(4) = 48
 */
seq_t a(idx_t n)
{
    if (n <= 4) {
        switch (n) {
            case 1: return 0;
            case 2: return 0;
            case 3: return 12;
            case 4: return 48;
            default: return 0; /* unreachable */
        }
    }
    seq_t nn = (seq_t) n;
    seq_t n_minus4 = nn - 4;
    seq_t n_minus2 = nn - 2;

    /* compute the formula using seq_t arithmetic */
    seq_t result = 8 * n_minus4 * n_minus4 + 16 * n_minus2 + 46 + 24 * n_minus2;
    return result;
}
