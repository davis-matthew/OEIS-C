/*
 * A099244  Greatest common divisor of length of n in binary representation
 *          and its number of ones.
 * https://oeis.org/A099244
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = gcd( bitlen(n), popcount(n) )
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sequence A099244:
 *   a(n) = gcd( length of n in binary, number of 1‑bits in n )
 * for n ≥ 1.  The computation uses only integer arithmetic.
 */
static seq_t gcd_u64(uint64_t a, uint64_t b)
{
    while (b != 0) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return (seq_t)a;
}

seq_t a(idx_t n)
{
    /* length of n in binary */
    uint64_t len = 1;
    idx_t tmp = n >> 1;          /* skip the most significant bit */
    while (tmp) {
        ++len;
        tmp >>= 1;
    }

    /* popcount of n */
    uint64_t ones = 0;
    tmp = n;
    while (tmp) {
        ones += tmp & 1;
        tmp >>= 1;
    }

    return gcd_u64(len, ones);
}
