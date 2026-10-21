/*
 * A206713  Number of solutions (n,k) of s(k)=s(n) (mod n), where 1<=k<n and
 *          s(k)=A000045(k+1) (Fibonacci numbers).
 * https://oeis.org/A206713
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # { 1 ≤ k < n : F_{k+1} ≡ F_{n+1} (mod n) } where F_m is
 *            the Fibonacci number with F_0 = 0, F_1 = 1
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;

    const seq_t mod = (seq_t)n;   /* modulus as seq_t */

    /* First pass: compute F_{n+1} mod n to obtain the target value. */
    seq_t f_prev = (seq_t)0;                 /* F_0 */
    seq_t f_curr = (seq_t)1;                 /* F_1 */
    seq_t target = f_curr;                  /* placeholder */

    for (idx_t i = 2; i <= n + 1; ++i) {
        seq_t f_next = (f_prev + f_curr) % mod; /* F_i */
        f_prev = f_curr;
        f_curr = f_next;
    }
    target = f_curr;                         /* F_{n+1} mod n */

    /* Second pass: count k in [1, n-1] with F_{k+1} ≡ target (mod n). */
    f_prev = (seq_t)0;                 /* F_0 */
    f_curr = (seq_t)1;                 /* F_1 */
    seq_t count = (seq_t)0;

    for (idx_t i = 2; i <= n; ++i) {
        seq_t f_next = (f_prev + f_curr) % mod; /* F_i */
        f_prev = f_curr;
        f_curr = f_next;

        /* Here f_curr is F_i, corresponding to k = i-1. */
        if (f_curr == target)
            ++count;
    }

    return count;
}
