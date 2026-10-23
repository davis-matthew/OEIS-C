/*
 * A011709  A binary m-sequence: expansion of reciprocal of x^8+x^7+x^2+x+1.
 * https://oeis.org/A011709
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for A011709:
   a(n) = a(n-1) ^ a(n-2) ^ a(n-7) ^ a(n-8)   (n ≥ 8)
   Initial values: a(0)=a(1)=a(2)=a(3)=a(4)=a(5)=a(6)=0, a(7)=1.
   The sequence is 255‑periodic. */
seq_t a(idx_t n) {
    static seq_t seq[255];
    static int ready = 0;
    if (!ready) {
        seq[0] = 0; seq[1] = 0; seq[2] = 0; seq[3] = 0;
        seq[4] = 0; seq[5] = 0; seq[6] = 0; seq[7] = 1;
        for (idx_t i = 8; i < 255; ++i) {
            seq[i] = seq[i-1] ^ seq[i-2] ^ seq[i-7] ^ seq[i-8];
        }
        ready = 1;
    }
    return seq[n % 255];
}
