/*
 * A021320  Decimal expansion of 1/316.
 * https://oeis.org/A021320
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: decimal expansion of 1/316 (period 13).
   First two digits are 0,0; thereafter the 13‑digit cycle
   3,1,6,4,5,5,6,9,6,2,0,2,5 repeats. */

seq_t a(idx_t n)
{
    /* 13‑digit repeating cycle starting at n = 2 */
    static const seq_t cycle[13] =
        {3, 1, 6, 4, 5, 5, 6, 9, 6, 2, 0, 2, 5};

    if (n < 2)
        return 0;
    return cycle[(n - 2) % 13];
}
