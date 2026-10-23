/*
 * A021130  Decimal expansion of 1/126.
 * https://oeis.org/A021130
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: digits of 1/126 in decimal
 * a(0) = 0, a(1) = 0, then a(n) cycles through {7,9,3,6,5,0} for n ≥ 2 */
seq_t a(idx_t n)
{
    /* The cycle of non‑zero digits (positions 2–7, 8–13, …) */
    static const seq_t cycle[6] = {7, 9, 3, 6, 5, 0};

    if (n < 2)
        return 0;                 /* a(0) and a(1) are the leading zeros */

    /* (n-2) is always ≥ 0; modulo 6 selects the correct cycle element */
    return cycle[(n - 2) % 6];
}
