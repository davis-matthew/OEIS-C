/*
 * A190974  a(n) = 7*a(n-1) - 5*a(n-2), with a(0)=0, a(1)=1.
 * https://oeis.org/A190974
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   * a(0) = 0 * a(1) = 1 * a(n) = 7*a(n-1) - 5*a(n-2) for n >= 2 *
 *            This function computes the terms of OEIS sequence A190974 *
 *            using only integer arithmetic, no dynamic memory allocation, *
 *            and works with arbitrary‑precision seq_t types as well as *
 *            built‑in integer types
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 25, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the two initialised terms directly. */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    /* Two previous terms: a(n-2) and a(n-1). */
    seq_t a0 = (seq_t)0;  /* a(0) */
    seq_t a1 = (seq_t)1;  /* a(1) */

    /* Compute iteratively up to the requested index. */
    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t next = ( (seq_t)7 * a1 ) - ( (seq_t)5 * a0 );
        a0 = a1;          /* shift window */
        a1 = next;
    }

    return a1;
}
