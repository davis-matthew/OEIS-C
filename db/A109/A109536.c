/*
 * A109536  a(0) = 1, a(n) = n+a(floor(n/2)) if n mod 2 = 0, a(n) =
 *          n-a(floor((n-1)/2)) if n mod 2 = 1.
 * https://oeis.org/A109536
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 143, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1; a(n)= n + a(n/2) if n even; a(n)= n - a((n-1)/2) if n odd */
seq_t a(idx_t n)
{
    if (n == 0) return 1;
    idx_t h = n >> 1;          /* floor(n/2) and floor((n-1)/2) both equal n>>1 */
    seq_t v = a(h);
    if (n & 1)                 /* n odd */
        return ((seq_t)n) - v;
    else                       /* n even */
        return ((seq_t)n) + v;
}
