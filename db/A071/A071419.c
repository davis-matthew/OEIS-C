/*
 * A071419  a(1)=1, a(n+1)=(a(n)+n)/2 if a(n)+n is even,
 *          a(n+1)=(3*(a(n)+n)+1)/2 otherwise.
 * https://oeis.org/A071419
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A071419
   a(1) = 1
   for n > 1:
     let s = a(n-1) + (n-1)
     a(n) = (s is even) ? (s / 2) : ((3 * s + 1) / 2)
   The code below computes a(n) iteratively for any n >= 1. */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    seq_t prev = (seq_t)1;                  /* a(1) */
    for (idx_t i = 1; i < n; ++i) {        /* i == n-1 in recurrence */
        seq_t sum = prev + (seq_t)i;        /* a(n-1) + (n-1) */
        if ((sum & 1) == 0)                /* even sum */
            prev = sum >> 1;                /* sum / 2 */
        else                               /* odd sum */
            prev = (sum * 3 + 1) >> 1;      /* (3*sum+1) / 2 */
    }
    return prev;
}
