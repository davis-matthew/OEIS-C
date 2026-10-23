/*
 * A011683  A binary m-sequence: expansion of reciprocal of
 *          x^7+x^5+x^4+x^3+x^2+x+1.
 * https://oeis.org/A011683
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) ^ a(n-2) ^ a(n-3) ^ a(n-4) ^ a(n-5) ^ a(n-7);
 *            a(0)=0, a(1)=0, a(2)=0, a(3)=0, a(4)=0, a(5)=0, a(6)=1
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static seq_t seq[127];
    static int initialized = 0;
    if (!initialized) {
        seq[0] = 0;
        seq[1] = 0;
        seq[2] = 0;
        seq[3] = 0;
        seq[4] = 0;
        seq[5] = 0;
        seq[6] = 1;
        for (idx_t i = 7; i < 127; ++i)
            seq[i] = seq[i-1] ^ seq[i-2] ^ seq[i-3] ^ seq[i-4] ^ seq[i-5] ^ seq[i-7];
        initialized = 1;
    }
    return seq[n % 127];
}
