/*
 * A163827  a(n) = 6n^3 + 1, solution z in Diophantine equation x^3 + y^3 =
 *          z^3 - 2. It may be considered a Fermat near miss by 2.
 * https://oeis.org/A163827
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*n^3 + 1
 * Range:     u8 n <= 3, u16 n <= 22, u32 n <= 894, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t val = nn * nn;   /* nn^2 */
    val = val * nn;        /* nn^3 */
    val = val * (seq_t)6;  /* 6*nn^3 */
    val += (seq_t)1;       /* +1 */
    return val;
}
