/*
 * A082498  a(0)=0, a(1)=1, a(2n)=a(n), a(2n+1)=a(n)+a(n-1).
 * https://oeis.org/A082498
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0, a(1)=1; a(2n)=a(n); a(2n+1)=a(n)+a(n-1) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;
    if ((n & 1) == 0)          /* even: a(2n)=a(n) */
        return a(n >> 1);
    else                       /* odd:  a(2n+1)=a(n)+a(n-1) */
        return a(n >> 1) + a((n >> 1) - 1);
}
