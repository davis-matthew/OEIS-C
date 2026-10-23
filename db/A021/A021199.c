/*
 * A021199  Decimal expansion of 1/195.
 * https://oeis.org/A021199
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

/* a(0)=0, a(1)=0, a(n) = [5,1,2,8,2,0][(n-2) mod 6] for n>=2 */
seq_t a(idx_t n)
{
    if (n < 2) return 0;
    static const seq_t cycle[6] = {
        (seq_t)5, (seq_t)1, (seq_t)2, (seq_t)8, (seq_t)2, (seq_t)0
    };
    idx_t idx = (n - 2) % 6;
    return cycle[idx];
}
