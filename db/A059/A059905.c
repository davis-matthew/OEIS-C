/*
 * A059905  Index of first half of decomposition of integers into pairs based
 *          on A000695.
 * https://oeis.org/A059905
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, look
 * Formula:   a(n) = Sum_{j>=0} bit(2j of n) * 2^j That is, take every second
 *            bit (starting with the least significant bit) of n and form a
 *            new integer from those bits. a(0)=0, a(1)=1, a(2)=0,
 * Range:     u8 n <= 8192, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS A059905 */
seq_t a(idx_t n)
{
    unsigned long long res = 0;          /* use 64‑bit type for safety   */
    unsigned long long tmp = n;          /* work on a copy of n          */
    unsigned long long pos = 0;          /* output bit position          */

    while (tmp) {                        /* repeat until all bits taken  */
        if (tmp & 1ULL)                  /* if the current input LSB is 1 */
            res |= 1ULL << pos;          /* set the corresponding output bit */
        tmp >>= 2;                       /* skip the next input bit (odd position) */
        pos++;
    }
    return (seq_t)res;                   /* cast to build‑specific type   */
}
