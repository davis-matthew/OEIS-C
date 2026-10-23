/*
 * A038669  [ n/2 ]+[ n/3 ]+[ n/4 ]+[ n/7 ]+[ n/11 ]+[ n/18 ]+[ n/29 ]+[ n/47
 *          ]+[ n/76 ]+[ n/123 ]+[ n/199 ]+... (using Lucas numbers A000032).
 * https://oeis.org/A038669
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ⌊n/2⌋ + ⌊n/3⌋ + ⌊n/4⌋ + ⌊n/7⌋ + ⌊n/11⌋ + ⌊n/18⌋ + ⌊n/29⌋
 *            + ⌊n/47⌋ + ⌊n/76⌋ + ⌊n/123⌋ + ⌊n/199⌋ + ... (Lucas numbers)
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;      /* cast index to sequence type for arithmetic   */
    seq_t sum = 0;
    seq_t l_prev = 2; /* Lucas L0 */
    seq_t l_curr = 1; /* Lucas L1 */

    /* Iterate over Lucas numbers until exceeds N; skip the divisor 1. */
    while (l_prev <= N) {
        if (l_prev != (seq_t)1)
            sum += N / l_prev;

        /* Compute next Lucas number: L_{k+2} = L_{k+1} + L_k          */
        seq_t l_next = l_prev + l_curr;
        l_prev = l_curr;
        l_curr = l_next;
    }
    return sum;
}
