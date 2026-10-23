/*
 * A105839  Numbers n such that n^2 = 19*m^2 + 19*m + 1.
 * https://oeis.org/A105839
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 115598*a(n-4) - a(n-8) Initial values: a(1) = 1 a(2) = 37
 *            a(3) = 2927 a(4) = 68171 a(5) = 183769 a(6) = 4280053 a(7) =
 *            338355383 a(8) = 7880431259
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* static table of the first eight terms, computed once */
    static seq_t base[8];
    static int initialized = 0;

    if (!initialized) {
        base[0] = (seq_t)1;
        base[1] = (seq_t)37;
        base[2] = (seq_t)2927;
        base[3] = (seq_t)68171;
        base[4] = (seq_t)115598 * base[0] + base[3];
        base[5] = (seq_t)115598 * base[1] + base[2];
        base[6] = (seq_t)115598 * base[2] + base[1];
        base[7] = (seq_t)115598 * base[3] + base[0];
        initialized = 1;
    }

    if (n <= 8)
        return base[n - 1];

    /* ring buffer holding the last eight computed terms. */
    seq_t ring[8];
    for (int i = 0; i < 8; ++i)
        ring[i] = base[i];

    for (idx_t k = 9; k <= n; ++k) {
        seq_t a_k_minus_4 = ring[(k - 5) & 7];  /* a(k-4) */
        seq_t a_k_minus_8 = ring[(k - 9) & 7];  /* a(k-8) */
        seq_t val = a_k_minus_4 * (seq_t)115598 - a_k_minus_8;
        ring[(k - 1) & 7] = val;                /* store a(k) */
    }

    return ring[(n - 1) & 7];
}
