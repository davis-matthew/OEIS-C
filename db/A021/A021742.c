/*
 * A021742  Decimal expansion of 1/738.
 * https://oeis.org/A021742
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = a(n-5) for n > 5, with a(0)=0, a(1)=0, a(2)=1, a(3)=3,
 *            a(4)=5, a(5)=5
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Reduce to the first six indices using the recurrence. */
    while (n >= 6)
        n -= 5;

    /* Return the corresponding base value. */
    switch (n) {
    case 0: return (seq_t)0;
    case 1: return (seq_t)0;
    case 2: return (seq_t)1;
    case 3: return (seq_t)3;
    case 4: return (seq_t)5;
    case 5: return (seq_t)5;
    default: return (seq_t)0; /* unreachable, keeps the compiler happy */
    }
}
