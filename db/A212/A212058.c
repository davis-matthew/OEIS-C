/*
 * A212058  Number of (w,x,y,z) with all terms in {1,...,n} and w>=x*y*z.
 * https://oeis.org/A212058
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=1..n+1} Sum_{j=1..n+1} tau(i)*floor((n+1-j)/i)
 *            (double‑sum definition, τ(i) is the divisor count of i)
 * Range:     u8 n <= 10, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Allocate an array to hold τ(1)..τ(n).  τ(n+1) is never used
       because floor((n+1-j)/i)=0 for i>n. */
    size_t len = (size_t)n + 2;                /* at least n+2 elements */
    uint64_t *tau = (uint64_t *)calloc(len, sizeof(uint64_t));
    if (!tau) return (seq_t)0;                  /* out‑of‑memory fallback */

    /* Compute divisor counts τ(i) for 1 ≤ i ≤ n */
    for (idx_t i = 1; i <= n; ++i) {
        for (idx_t j = i; j <= n; j += i) {
            tau[j] += 1;
        }
    }

    seq_t total = 0;

    /* Use the closed‑form for the inner sum:  Σ_{k=0}^n ⌊k/i⌋
       = i·t·(t–1)/2 + (n – i·t + 1)·t   where t = ⌊n/i⌋. */
    for (idx_t i = 1; i <= n; ++i) {
        idx_t t = n / i;                      /* t = ⌊n/i⌋ */
        if (t == 0) continue;                 /* contributes nothing */

        seq_t i_s = (seq_t)i;
        seq_t t_s = (seq_t)t;

        seq_t inner = i_s * t_s * (t_s - 1) / 2;              /* i·t·(t–1)/2 */
        inner += t_s * ((seq_t)n - t_s * i_s + 1);            /* + (n−i·t+1)·t */

        total += (seq_t)tau[i] * inner;
    }

    free(tau);
    return total;
}
