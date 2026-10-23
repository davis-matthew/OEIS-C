/*
 * A197652  Numbers that are congruent to 0 or 1 mod 10.
 * https://oeis.org/A197652
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((n-1)/2)*10 + ((n-1)%2)
 * Range:     u8 n <= 52, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This produces numbers congruent to 0 or 1 modulo 10, starting with a(1)=0. */
seq_t a(idx_t n)
{
    if (n <= 0)
        return 0;                     /* out of bounds, return 0 */

    idx_t k = n - 1;                   /* 0‑based index of the term */
    seq_t sq_k = (seq_t)k;             /* convert to seq_t for arithmetic */
    seq_t m   = sq_k / 2;              /* floor((n-1)/2) */
    seq_t r   = sq_k % 2;              /* (n-1) mod 2 -> 0 or 1 */

    return m * 10 + r;                 /* 10*floor + remainder */
}
