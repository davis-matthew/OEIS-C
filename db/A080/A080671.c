/*
 * A080671  Numbers having divisors 2 or 3 or 5.
 * https://oeis.org/A080671
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 187, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers having divisors 2 or 3 or 5.
   a(1) = 2.
   The set of such numbers is periodic modulo 30 with 22 members per period:
   2,3,4,5,6,8,9,10,12,14,15,16,18,20,21,22,24,25,26,27,28,30.
   Hence a(n+22) = a(n) + 30. */
seq_t a(idx_t n)
{
    static const seq_t offsets[22] = {
        2,3,4,5,6,8,9,10,12,14,15,16,18,20,21,22,24,25,26,27,28,30
    };
    if (n == 0) return 0;          /* undefined for n<1, return 0 as guard */
    idx_t block      = (n - 1) / 22;   /* number of complete 30‑blocks */
    idx_t offset_idx = (n - 1) % 22;   /* position inside the current block */
    seq_t result = (seq_t)block * 30 + offsets[offset_idx];
    return result;
}
