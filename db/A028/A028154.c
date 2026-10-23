/*
 * A028154  Expansion of 1/((1-4x)(1-7x)(1-11x)(1-12x)).
 * https://oeis.org/A028154
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A028154: Expansion of 1/((1-4x)(1-7x)(1-11x)(1-12x))
   Recurrence:
     a(0) = 1
     a(1) = 34
     a(2) = 743
     a(3) = 13316
     a(n) = 34*a(n-1) - 413*a(n-2) + 2096*a(n-3) - 3696*a(n-4)   for n ≥ 4
   This implementation uses only the arithmetic operators allowed on seq_t
   and works in both C (where seq_t is an unsigned integer type) and
   C++ (where seq_t may be an arbitrary‑precision integer class). */

seq_t a(idx_t n)
{
    /* handle the first four terms directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)34;
    if (n == 2) return (seq_t)743;
    if (n == 3) return (seq_t)13316;

    /* initialise the four previous values */
    seq_t a0 = (seq_t)1;      /* a(n‑4)   */
    seq_t a1 = (seq_t)34;     /* a(n‑3)   */
    seq_t a2 = (seq_t)743;    /* a(n‑2)   */
    seq_t a3 = (seq_t)13316;  /* a(n‑1)   */

    /* iterate from 4 up to n */
    for (idx_t i = 4; i <= n; ++i)
    {
        seq_t next = (seq_t)34 * a3
                    - (seq_t)413 * a2
                    + (seq_t)2096 * a1
                    - (seq_t)3696 * a0;
        /* shift the window of previous values */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }

    return a3;
}
