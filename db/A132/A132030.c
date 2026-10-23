/*
 * A132030  a(n) = Product_{k=0..floor(log_6(n))} floor(n/6^k), n>=1.
 * https://oeis.org/A132030
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{k=0..⌊log_6 n⌋} ⌊n/6^k⌋ (OEIS A132030)
 * Range:     u8 n <= 41, u16 n <= 251, u32 n <= 3215, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = n * a(n/6), with a(1) = 1 (n≥1) */

seq_t a(idx_t n)
{
    seq_t result = 1;
    for (idx_t t = n; t > 0; t /= 6)
    {
        result *= (seq_t)t;
    }
    return result;
}
