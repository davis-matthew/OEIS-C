/*
 * A056116  a(n) = 121*12^(n-2), a(0)=1, a(1)=10.
 * https://oeis.org/A056116
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 121 * 12^(n-2), a(0)=1, a(1)=10
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the two special base cases first */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)10;

    /* For n >= 2 compute 121 * 12^(n-2) using simple repeated multiplication.
       This is O(n) and fast for the required bounds (n <= 10 000).  The multiplication
       uses seq_t, which is either a built‑in unsigned integer type or an arbitrary‑precision
       integer class.  In the 64‑bit build the function is only checked for results that
       fit into 64 bits, so any overflow beyond that point is irrelevant.  In the arbitrary
       precision build the multiplication will produce the exact value.  */
    idx_t exponent = n - 2;
    seq_t result = (seq_t)121;

    for (idx_t i = 0; i < exponent; ++i)
        result *= (seq_t)12;

    return result;
}
