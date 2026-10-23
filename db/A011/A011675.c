/*
 * A011675  A binary m-sequence: expansion of reciprocal of x^7+x^4+1.
 * https://oeis.org/A011675
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-7) XOR a(n-4) (mod 2) Initialised with
 *            a(0)=0,a(1)=0,a(2)=0,a(3)=0,a(4)=0,a(5)=0,a(6)=1 The sequence is
 *            127‑periodic
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The seed (a(0) … a(6)) */
    const seq_t seed[7] = {0, 0, 0, 0, 0, 0, 1};

    /* Reduce n modulo the period (127) */
    idx_t k = n % 127;
    if (k < 7)
        return seed[k];

    /* Circular buffer of the last 7 bits */
    seq_t state[7];
    for (idx_t i = 0; i < 7; ++i)
        state[i] = seed[i];

    /* Compute the recurrence up to the required index k */
    for (idx_t i = 7; i <= k; ++i)
    {
        seq_t next = state[(i - 7) % 7] ^ state[(i - 4) % 7];
        state[i % 7] = next;
    }

    return state[k % 7];
}
