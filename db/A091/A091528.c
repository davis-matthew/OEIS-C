/*
 * A091528  a(n) = (Sum_{k=1..n} H(k)*k!*(n-k)!) mod (n+1), where H(k) is the
 *          k-th harmonic number.
 * https://oeis.org/A091528
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} H(k)*k!*(n-k)! mod (n+1)
 * Range:     u8 n <= 95, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for offset 1, but safe */

    seq_t mod = (seq_t)(n + 1);
    size_t N = (size_t)n;

    /* factorial modulo (n+1) for 0..n */
    seq_t *fact = (seq_t*)malloc((N + 1) * sizeof(seq_t));
    if (!fact) return 0;           /* fallback if allocation fails */

    fact[0] = (seq_t)1 % mod;
    for (size_t i = 1; i <= N; ++i)
        fact[i] = (fact[i - 1] * (seq_t)i) % mod;

    seq_t sum = 0;
    seq_t A_prev = 1 % mod;        /* A_1 = H(1)*1! = 1 */

    for (size_t k = 1; k <= N; ++k) {
        /* term = A_k * (n-k)!  (mod mod) */
        seq_t term = (A_prev * fact[N - k]) % mod;
        sum += term;
        if (sum >= mod) sum -= mod;   /* keep sum < mod */

        if (k == N) break;            /* last term processed */

        /* compute A_{k+1} = (k+1)*A_k + k!   (mod mod) */
        seq_t k1 = (seq_t)(k + 1);
        seq_t A_next = (k1 * A_prev + fact[k]) % mod;
        A_prev = A_next;
    }

    free(fact);
    return sum % mod;
}
