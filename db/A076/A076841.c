/*
 * A076841  a(1) = a(2) = 1; a(n) = (a(n-1)+1)/a(n-2) (for n>2, n odd),
 *          (a(n-1)^3+1)/a(n-2) (for n>2, n even).
 * https://oeis.org/A076841
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A076841: a(1)=a(2)=1; for n>2,
   a(n) = (a(n-1)+1)/a(n-2)  if n odd,
   a(n) = (a(n-1)^3+1)/a(n-2) if n even.
   This sequence is periodic with period 8:
   {1, 1, 2, 9, 5, 14, 3, 2}. */

seq_t a(idx_t n)
{
    /* 8‑term period of the sequence */
    static const seq_t period[8] = {
        (seq_t)1, (seq_t)1, (seq_t)2, (seq_t)9,
        (seq_t)5, (seq_t)14, (seq_t)3, (seq_t)2
    };

    /* Period 8: a(n) = period[(n-1) mod 8] */
    return period[(n - 1) % 8];
}
