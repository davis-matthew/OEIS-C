/*
 * A100168  Structured pentagonal icositetrahedral numbers (vertex structure
 *          10).
 * https://oeis.org/A100168
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 18, u32 n <= 742, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 4*a(n-1) - 6*a(n-2) + 4*a(n-3) - a(n-4);
   a(1)=1, a(2)=38, a(3)=174, a(4)=472. */

seq_t a(idx_t n)
{
    /* Handle the first four values directly */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)38;
    if (n == 3) return (seq_t)174;
    if (n == 4) return (seq_t)472;

    seq_t a1 = (seq_t)1;   /* a(n-4)   */
    seq_t a2 = (seq_t)38;  /* a(n-3)   */
    seq_t a3 = (seq_t)174; /* a(n-2)   */
    seq_t a4 = (seq_t)472; /* a(n-1)   */

    for (idx_t i = 5; i <= n; ++i)
    {
        seq_t next = 4*a4 - 6*a3 + 4*a2 - a1;

        /* shift window */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = next;
    }

    return a4;
}
