/*
 * A011660  A binary m-sequence: expansion of reciprocal of x^5+x^4+x^2+x+1.
 * https://oeis.org/A011660
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-5) + a(n-4) + a(n-2) + a(n-1) (mod 2)
 * (i.e. XOR of the 5‑term window).  The sequence is periodic with
 * period 31.  The first 31 terms are
 * 0,0,0,0,1,1,0,1,0,1,0,0,1,0,0,0,1,0,1,1,1,1,1,0,1,1,0,0,1,1,1.  */

/* The function is fully independent of any build‑specific utilities. */
seq_t a(idx_t n)
{
    static seq_t pattern[31];
    static int initialized = 0;

    if (!initialized) {
        /* first 5 terms */
        pattern[0] = 0;
        pattern[1] = 0;
        pattern[2] = 0;
        pattern[3] = 0;
        pattern[4] = 1;

        /* sliding window of last 5 terms:
           t[0] = a(k-5), t[1] = a(k-4), t[2] = a(k-3),
           t[3] = a(k-2), t[4] = a(k-1).  */
        seq_t t[5] = {0, 0, 0, 0, 1};

        idx_t i;
        for (i = 5; i < 31; ++i) {
            /* a(k) = t4 ^ t3 ^ t1 ^ t0 (XOR over GF(2)) */
            seq_t val = t[4] ^ t[3] ^ t[1] ^ t[0];
            pattern[i] = val;

            /* shift window for next step */
            t[0] = t[1];
            t[1] = t[2];
            t[2] = t[3];
            t[3] = t[4];
            t[4] = val;
        }
        initialized = 1;
    }
    /* period 31 */
    return pattern[(unsigned)(n % 31)];
}
