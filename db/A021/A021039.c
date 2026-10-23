/*
 * A021039  Decimal expansion of 1/35.
 * https://oeis.org/A021039
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0)=0; a(n)=2,8,5,7,1,4 repeating for n≥1 (digits of 1/35) */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    static const seq_t cycle[6] = {2, 8, 5, 7, 1, 4};
    return cycle[(n - 1) % 6];
}
