/*
 * A021210  Decimal expansion of 1/206.
 * https://oeis.org/A021210
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

/* a(n) is the nth digit of the decimal expansion of 1/206 (offset 0).
   Long division yields the recurrence:
     r_0 = 1
     r_{k+1} = (10 * r_k) mod 206
     a(k)  = floor( (10 * r_k) / 206 )
   This implementation runs in O(n) time and uses only integer arithmetic suitable
   for any seq_t. */
seq_t a(idx_t n)
{
    static const seq_t MOD = (seq_t)206;   /* denominator */
    seq_t r = (seq_t)1;                   /* r_0 */

    for (idx_t i = 0; i < n; ++i) {
        r = (r * (seq_t)10) % MOD;
    }
    seq_t digit = (r * (seq_t)10) / MOD;
    return digit;
}
