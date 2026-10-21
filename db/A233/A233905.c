/*
 * A233905  a(2n) = a(n), a(2n+1) = a(n) + n, with a(0)=0.
 * https://oeis.org/A233905
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2n) = a(n), a(2n+1) = a(n)+n, a(0)=0
 * Range:     u8 n <= 286, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the binary‑bits summation: a(n) = Σ_{bit_k(n)=1} floor(n/2^(k+1)) */
seq_t a(idx_t n)
{
    seq_t res = 0;
    idx_t b = n;
    while (b) {
        if (b & 1)
            res += (seq_t)(b >> 1);
        b >>= 1;
    }
    return res;
}
