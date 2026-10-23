/*
 * A022524  Nexus numbers (n+1)^8 - n^8.
 * https://oeis.org/A022524
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)^8 - n^8
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 17, u64 n <= 419,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_t  = (seq_t)n;          /* n as seq_t */
    seq_t np1  = n_t + 1;           /* n+1 */

    /* compute n^8 */
    seq_t n2   = n_t * n_t;         /* n^2 */
    seq_t n4   = n2  * n2;          /* n^4 */
    seq_t n8   = n4  * n4;          /* n^8 */

    /* compute (n+1)^8 */
    seq_t np12 = np1 * np1;         /* (n+1)^2 */
    seq_t np14 = np12 * np12;       /* (n+1)^4 */
    seq_t np18 = np14 * np14;       /* (n+1)^8 */

    /* difference */
    return np18 - n8;
}
