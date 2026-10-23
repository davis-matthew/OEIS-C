/*
 * A125916  Sprague-Grundy values for octal game .15 (Guiles).
 * https://oeis.org/A125916
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

/* Periodic sequence: a(0)=0, a(1)=1, a(2)=1, a(3)=0, a(4)=1, a(5)=1,
   a(6)=2, a(7)=2, a(8)=1, a(9)=2, a(10)=2; for n>10, a(n)=a(n-10). */
seq_t a(idx_t n)
{
    /* Values for positions 0 .. 10 */
    static const seq_t seq[11] = {
        0, /* a(0) */
        1, /* a(1) */
        1, /* a(2) */
        0, /* a(3) */
        1, /* a(4) */
        1, /* a(5) */
        2, /* a(6) */
        2, /* a(7) */
        1, /* a(8) */
        2, /* a(9) */
        2  /* a(10) */
    };

    if (n <= 10)
        return seq[n];

    /* For n > 10 use the periodicity: a(n) = a(n-10).  Compute the
       equivalent index in the range 1..10. */
    idx_t idx = ((n - 1U) % 10U) + 1U;
    return seq[idx];
}
