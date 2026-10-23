/*
 * A107680  Repeating k-th ternary repunit (A003462) 2^k times, k >= 0.
 * https://oeis.org/A107680
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A003462(A000523(n+1))
 * Range:     u8 n <= 31, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A000523(m) = interpretation of binary(m-1) in base 3 */
/*  A003462(k) = (3^k - 1) / 2 */

seq_t a(idx_t n)
{
    /* Step 1: compute m = A000523(n+1)
       Interpret the binary representation of n in base 3. */
    seq_t m = 0;
    seq_t pow3 = 1;
    idx_t temp = n;
    while (temp != 0) {
        if (temp & 1) {
            m += pow3;
        }
        pow3 *= (seq_t)3;   /* multiply current power of 3 by 3 */
        temp >>= 1;
    }

    /* Step 2: compute t = 2*m + 1 */
    seq_t t = m;
    t += m;
    t += (seq_t)1;

    /* Step 3: find largest power of 3 <= t */
    seq_t p = 1;
    while (p * (seq_t)3 <= t) {
        p *= (seq_t)3;
    }

    /* Step 4: a = (p - 1) / 2 = (3^k - 1)/2 */
    seq_t result = (p - (seq_t)1) / (seq_t)2;
    return result;
}
