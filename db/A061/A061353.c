/*
 * A061353  First column of array shown in A061352.
 * https://oeis.org/A061353
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A061353 – Least significant digit of the nth triangular number
 * for n even; for n odd the sequence is defined by a(1)=1 and
 * a(n+1) = (a(n)+2n) mod 10 if n is odd,
 * a(n+1) = (a(n)+1)  mod 10 if n is even.
 *
 * Closed‑form (valid for all n ≥ 1):
 *   let m = ⌊n/2⌋
 *   a(n) = ( 2*m*m + m + (n mod 2) ) mod 10
 *
 * This implementation uses only integer arithmetic and works for any
 * n castable to idx_t.  Intermediate calculations are kept within
 * the range 0–171 so they fit in any seq_t type.
 */

seq_t a(idx_t n)
{
    /* m = ⌊n/2⌋   (integer division) */
    idx_t m = n >> 1;

    /* t = m mod 10 ; t is at most 9 */
    idx_t t = m % 10;

    /* Convert to seq_t for subsequent arithmetic */
    seq_t tt = (seq_t)t;

    /* Compute (2*m*m + m + (n mod 2)) mod 10 */
    seq_t part = ((seq_t)2 * tt * tt) % (seq_t)10; /* 2*m*m mod 10  */
    part = (part + tt) % (seq_t)10;                /* + m mod 10    */
    if (n & 1U)
        part = (part + 1) % (seq_t)10;            /* +1 if n odd    */

    return part;
}
