/*
 * A167534  a(n) = 79*n - a(n-1) for n>0, a(0)=9.
 * https://oeis.org/A167534
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 9; a(n) = 79*n - a(n-1) for n>0. Closed‑form: a(n) =
 *            79*(n/2) + (n % 2 ? 70 : 9)
 * Range:     u8 n <= 6, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t half = n / 2;                     /* floor(n/2) */
    seq_t res  = (seq_t)79 * (seq_t)half;    /* 79 * floor(n/2) */

    if (n & 1)                                /* odd n */
        res += (seq_t)70;
    else                                      /* even n */
        res += (seq_t)9;

    return res;
}
