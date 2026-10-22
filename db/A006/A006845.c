/*
 * A006845  State assignments for n-state machine.
 * https://oeis.org/A006845
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2^k - 1)! / ((2^k - n)! * k!) with k = ceil(log_2(n)),
 *            for n >= 2; a(1) = 0
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 16, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n <= 1) return 0;

    /* k = smallest integer with 2^k >= n */
    idx_t k = 0;
    while (k < 63 && ((idx_t)1 << k) < n) k++;
    idx_t N = (idx_t)1 << k;          /* 2^k */
    idx_t m = n - 1;                   /* number of factors in (2^k-1)!/(2^k-n)! */
    idx_t base = N - n;                /* factors are base+1 .. base+m = 2^k-1 */

    /* The product of the first i consecutive integers is divisible by i!,
       so dividing by i during the first k steps stays exact. */
    seq_t r = 1;
    for (idx_t i = 1; i <= m; i++) {
        r *= (seq_t)(base + i);
        if (i <= k) r /= (seq_t)i;
    }
    return r;
}
