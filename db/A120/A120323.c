/*
 * A120323  Periodic sequence 0, 3, 1, 0, 1, 3.
 * https://oeis.org/A120323
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (4/3)*(sin(n*Pi/6)+sin(n*Pi/2))^2
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is periodic with period 6:
       index 0 1 2 3 4 5  repeated as 0,3,1,0,1,3 */
    static const seq_t table[6] = {
        (seq_t)0, (seq_t)3, (seq_t)1, (seq_t)0, (seq_t)1, (seq_t)3
    };
    /* Using idx_t modulo to find position within the period */
    idx_t mod = n % 6;
    return table[mod];
}
