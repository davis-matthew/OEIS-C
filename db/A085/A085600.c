/*
 * A085600  Number of simple graphs with 3 edges on n unlabeled vertices.
 * https://oeis.org/A085600
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0 for n = 1,2; 1 for n = 3; 3 for n = 4; 4 for n = 5; 5
 *            for n ≥ 6
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;              /* undefined for n < 1, return 0 */
    switch ((idx_t)n) {
        case 1: return (seq_t)0;
        case 2: return (seq_t)0;
        case 3: return (seq_t)1;
        case 4: return (seq_t)3;
        case 5: return (seq_t)4;
        default: return (seq_t)5;      /* n ≥ 6 */
    }
}
