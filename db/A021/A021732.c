/*
 * A021732  Decimal expansion of 1/728.
 * https://oeis.org/A021732
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = a(n-1) - a(n-3) + a(n-4) for n > 6 with initial values
 *            a(0) = 0, a(1) = 0, a(2) = 1, a(3) = 3, a(4) = 7, a(5) = 3, a(6)
 *            = 6
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* first few terms, handled explicitly */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)3;
    if (n == 4) return (seq_t)7;
    if (n == 5) return (seq_t)3;
    if (n == 6) return (seq_t)6;

    /* for n > 6 the sequence is periodic with period 6:
       n % 6 == 0 -> 6
       n % 6 == 1 -> 2
       n % 6 == 2 -> 6
       n % 6 == 3 -> 3
       n % 6 == 4 -> 7
       n % 6 == 5 -> 3   */
    switch (n % 6)
    {
        case 0: return (seq_t)6;
        case 1: return (seq_t)2;
        case 2: return (seq_t)6;
        case 3: return (seq_t)3;
        case 4: return (seq_t)7;
        case 5: return (seq_t)3;
        default: return (seq_t)0; /* unreachable */
    }
}
