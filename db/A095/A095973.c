/*
 * A095973  Yard markers on a U.S.A. football field.
 * https://oeis.org/A095973
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = 10 * (5 - |5 - (n mod 10)|)
 * Range:     u8 n <= 10, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS A095973: yard markers on a U.S.A. football field. */
seq_t a(idx_t n)
{
    /* n mod 10 gives the position within a 10‑yard block (0…9). */
    seq_t k = (seq_t)(n % (idx_t)10);

    /* Compute the absolute difference |5 - k|. */
    seq_t diff = k > 5 ? k - 5 : 5 - k;

    /* The value is 5 minus that difference, then scaled by 10. */
    seq_t val = 5 - diff;     /* 0…5 */
    return val * (seq_t)10;   /* 0,10,20,30,40,50,40,30,20,10,0,10,20,… */
}
