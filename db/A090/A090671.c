/*
 * A090671  Decreases from 10^k - 1 down to 1, restarting at 10^(k+1) - 1, for
 *          k >= 1.
 * https://oeis.org/A090671
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A090671: a(n) = 10^k - n', where k is the smallest integer such that
   n ≤ Σ_{i=1}^k (10^i – 1) and n' = n – Σ_{i=1}^{k-1} (10^i – 1). */

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset is 1; 0 is unused */

    seq_t cum = 0;                    /* cumulative count of terms up to previous block */
    seq_t pow10 = 10;                 /* 10^1 */

    for (;;) {
        seq_t block_len = pow10 - 1;  /* length of the current block (10^k – 1) */
        seq_t cum_next = cum + block_len;

        if ((seq_t)n <= cum_next) {  /* n lies within this block */
            seq_t offset = (seq_t)n - cum;   /* 1 … block_len */
            return pow10 - offset;          /* 10^k – offset = a(n) */
        }

        cum = cum_next;                /* advance to next block */
        pow10 = pow10 * 10;            /* next power of ten */
    }
}
