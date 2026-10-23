/*
 * A018824  n is the sum of k nonzero squares for all 5 <= k <= n-14 (contains
 *          all integers >= 19 except 33).
 * https://oeis.org/A018824
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * a(n) = n + 18 for n ≤ 14
 * a(n) = n + 19 for n > 14
 * (sequence enumerates all integers ≥ 19 except 33)
 */
seq_t a(idx_t n)
{
    /* convert index to seq_t for arithmetic */
    seq_t res = (seq_t)n + (seq_t)18;
    if (n > (idx_t)14)
        res += (seq_t)1;   /* skip the integer 33 */
    return res;
}
