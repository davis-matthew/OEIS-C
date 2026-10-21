/*
 * A055920  Susceptibility series H_4 for 2-dimensional Ising model (divided
 *          by 2) for 1 particle excitation.
 * https://oeis.org/A055920
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 12, u32 n <= 121, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(0) = 1.
 * For n > 0:
 *   if n is even: a(n) = (133*n^4 + 524*n^2 + 96) / 48
 *   if n is odd : a(n) = (133*n^4 + 542*n^2 + 93) / 48
 * The computation uses only integer arithmetic on seq_t.
 */
seq_t a(idx_t n)
{
    if (n == 0) {
        return 1;
    }

    /* convert index to seq_t once */
    seq_t ns = (seq_t)n;

    /* compute n^2 and n^4 using seq_t arithmetic */
    seq_t n2 = ns * ns;      /* n^2 */
    seq_t n4 = n2 * n2;      /* n^4 */

    seq_t result;
    if ((n & 1) == 0) { /* even n */
        result = (133 * n4 + 524 * n2 + 96) / 48;
    } else {            /* odd n */
        result = (133 * n4 + 542 * n2 + 93) / 48;
    }
    return result;
}
