/*
 * A060901  Exact power of 3 that divides the n-th Fibonacci number (sequence
 *          A000045).
 * https://oeis.org/A060901
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 if n mod 4 ≠ 0, else 3^(v₃(n/4)+1)
 * Range:     u8 n <= 971, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the exact power of 3 dividing the n‑th Fibonacci number. */
seq_t a(idx_t n)
{
    /* n=1 gives 1 as required. */
    if (n == 0) return 1;          /* safety for illegal call */

    /* If n is not a multiple of 4, the answer is 1. */
    if (n % 4 != 0) return 1;

    /* Count the exponent of 3 in n/4. */
    idx_t t = n >> 2;               /* n / 4 */
    int k = 0;
    while (t % 3 == 0) {
        t /= 3;
        ++k;
    }

    /* Compute 3^(k+1). */
    seq_t res = 3;                   /* base case: k = 0 -> 3^1 */
    for (int i = 0; i < k; ++i) {
        res *= 3;
    }
    return res;
}
