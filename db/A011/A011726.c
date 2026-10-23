/*
 * A011726  A binary m-sequence: expansion of reciprocal of x^13+x^4+x^3+x+1.
 * https://oeis.org/A011726
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-13) XOR a(n-4) XOR a(n-3) XOR a(n-1) over GF(2)
 *            (binary m‑sequence of period 8191). Initial values:
 *            a(0)=a(1)=…=a(11)=0, a(12)=a(13)=a(14)=1
 * Range:     u8 n <= 9000, u16 n <= 9000, u32 n <= 9000, u64 n <= 9000,
 *            u128 n <= 9000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    enum { PERIOD = 8191 };
    static seq_t table[PERIOD];
    static int initialized = 0;
    if (!initialized) {
        /* Set the first 15 terms explicitly */
        table[0]  = 0; table[1]  = 0; table[2]  = 0; table[3]  = 0; table[4]  = 0;
        table[5]  = 0; table[6]  = 0; table[7]  = 0; table[8]  = 0; table[9]  = 0;
        table[10] = 0; table[11] = 0; table[12] = 1; table[13] = 1; table[14] = 1;
        /* Compute the remaining terms of the period using the recurrence */
        for (idx_t i = 15; i < PERIOD; ++i) {
            table[i] = table[i - 13] ^ table[i - 4] ^ table[i - 3] ^ table[i - 1];
        }
        initialized = 1;
    }
    idx_t idx = n % PERIOD;
    return table[idx];
}
