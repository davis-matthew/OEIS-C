/*
 * A076840  a(1) = a(2) = 1; a(n) = (a(n-1) + 1)/a(n-2) (for n>2, n odd),
 *          (a(n-1)^2 + 1)/a(n-2) (for n>2, n even).
 * https://oeis.org/A076840
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = a(2) = 1; a(n) = (a(n-1)+1)/a(n-2) when n is odd, a(n) =
 *            (a(n-1)^2+1)/a(n-2) when n is even. The recurrence yields a
 *            period 6: 1,1,2,5,3,2 repeated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* period 6 pattern: n%6 == 1 ->1, 2->1, 3->2, 4->5, 5->3, 0->2 */
    switch ((int)(n % 6))
    {
        case 1: return (seq_t)1;
        case 2: return (seq_t)1;
        case 3: return (seq_t)2;
        case 4: return (seq_t)5;
        case 5: return (seq_t)3;
        case 0: return (seq_t)2;
        default: return (seq_t)0; /* unreachable */
    }
}
