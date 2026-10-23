/*
 * A023541  Convolution of natural numbers with Beatty sequence for the golden
 *          mean A000201.
 * https://oeis.org/A023541
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1..n} k * floor((n+1-k) * φ), where φ = (1+√5)/2.
 *            Beatty sequence B(m) = floor(m * φ) = m + floor(m/φ). To avoid
 *            floating-point, we use the identity floor(m/φ) = floor(m * F_k /
 *            F_{k+1}), where F_k and F_{k+1} are consecutive Fibonacci
 *            numbers such that F_{k+1} > m. This gives exact i...
 * Range:     u8 n <= 9, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n < 1) return 0; /* out of OEIS range, but safe */

    /* Find Fibonacci numbers F_k and F_{k+1} with F_{k+1} > n. */
    uint64_t f_prev = 0;   /* F_0 */
    uint64_t f_curr = 1;   /* F_1 */
    uint64_t F_k = 0;      /* will hold F_k */
    uint64_t F_kp1 = 0;    /* will hold F_{k+1} */
    while (f_curr <= n) {
        uint64_t f_next = f_prev + f_curr;
        f_prev = f_curr;
        f_curr = f_next;
    }
    /* after loop: f_curr > n, f_prev <= n, thus
       F_k = f_prev, F_{k+1} = f_curr */
    F_k   = f_prev;
    F_kp1 = f_curr;

    /* Compute a(n) = Σ_{i=1..n} (n+1-i) * B(i)  with
       B(i) = i + (i * F_k) / F_{k+1}. */
    seq_t sum = 0;
    for (idx_t i = 1; i <= n; ++i) {
        /* compute B(i) */
        seq_t bi = (seq_t)i + ((seq_t)i * (seq_t)F_k) / (seq_t)F_kp1;
        idx_t k  = n + 1 - i;           /* the multiplier */
        sum += (seq_t)k * bi;
    }
    return sum;
}
