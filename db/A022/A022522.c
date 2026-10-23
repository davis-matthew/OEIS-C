/*
 * A022522  Nexus numbers (n+1)^6 - n^6.
 * https://oeis.org/A022522
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)^6 - n^6
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 58, u64 n <= 4983,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast to seq_t before doing arithmetic to avoid wrap‑around of idx_t */
    seq_t a = (seq_t)n + 1;
    seq_t b = (seq_t)n;

    /* compute a^6 and b^6 multiplicatively to keep intermediate values small */
    seq_t a2 = a * a;
    seq_t a3 = a2 * a;
    seq_t a6 = a3 * a3;

    seq_t b2 = b * b;
    seq_t b3 = b2 * b;
    seq_t b6 = b3 * b3;

    return a6 - b6;
}
