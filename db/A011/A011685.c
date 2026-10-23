/*
 * A011685  A binary m-sequence: expansion of reciprocal of x^7+x^6+x^3+x+1.
 * https://oeis.org/A011685
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-7) + a(n-6) + a(n-3) + a(n-1) modulo 2 with a(0)=0,
 *            a(1)=0, a(2)=0, a(3)=0, a(4)=0, a(5)=0, a(6)=1. Sequence is
 *            127–periodic
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    static seq_t seq127[127];
    static int initialized = 0;
    if (!initialized) {
        seq127[0] = 0;
        seq127[1] = 0;
        seq127[2] = 0;
        seq127[3] = 0;
        seq127[4] = 0;
        seq127[5] = 0;
        seq127[6] = 1;
        for (idx_t i = 7; i < 127; ++i) {
            seq127[i] = seq127[i-7] ^ seq127[i-6] ^ seq127[i-3] ^ seq127[i-1];
        }
        initialized = 1;
    }
    return seq127[n % 127];
}
