/*
 * A129308  a(n) is the number of positive integers k such that k*(k+1)
 *          divides n.
 * https://oeis.org/A129308
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of k ≥ 1 such that k*(k+1) divides n (computed by
 *            brute‑force checking each oblong number <= n)
 * Range:     u8 n <= 407, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast n to seq_t for operations involving arbitrary-precision types */
    seq_t nn = (seq_t)n;
    seq_t count = 0;
    for (idx_t k = 1;; ++k) {
        seq_t kk = (seq_t)k;
        seq_t prod = kk * (kk + 1);
        if (prod > nn) break;          /* k(k+1) exceeds n */
        if (nn % prod == 0) ++count;   /* oblong divides n */
    }
    return count;
}
