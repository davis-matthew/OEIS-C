/*
 * A147960  a(n) = ((9 + sqrt(2))^n + (9 - sqrt(2))^n)/2.
 * https://oeis.org/A147960
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A147960:
   a(n) = ((9 + sqrt(2))^n + (9 - sqrt(2))^n)/2
   Recurrence: a(0)=1, a(1)=9, for n≥2: a(n)=18*a(n-1)-79*a(n-2) */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)9;

    seq_t prev2 = (seq_t)1; /* a(n-2) */
    seq_t prev1 = (seq_t)9; /* a(n-1) */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i) {
        cur = (seq_t)18 * prev1 - (seq_t)79 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
