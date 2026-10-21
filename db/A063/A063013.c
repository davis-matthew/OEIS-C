/*
 * A063013  Numbers where k-th digit from right is either 0 or k.
 * https://oeis.org/A063013
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn, fini, full
 * Formula:   a(n) = Σ_{k ≥ 0, bit_k(n)=1} (k+1)·10^k where bit_k(n) is the
 *            k‑th least significant bit of n (0‑based). This directly
 *            implements the OEIS definition: the k‑th digit from the right of
 *            a(n) is either 0 or k
 * Range:     u8 n <= 3, u16 n <= 31, u32 n <= 511, u64 n <= 511,
 *            u128 n <= 511, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t bits = n;          /* work with a copy of n */
    seq_t result = 0;        /* accumulated value of a(n) */
    seq_t pow10   = 1;       /* 10^k for current bit position k */
    seq_t digit   = 1;       /* (k+1) for current bit position k */

    while (bits) {
        if (bits & 1ULL)                /* set bit at position k ? */
            result += digit * pow10;    /* add (k+1)·10^k if bit is set */
        bits >>= 1;                     /* move to next bit position */
        pow10 *= 10;                    /* next power of 10 */
        digit += 1;                     /* next coefficient (k+1) */
    }
    return result;
}
