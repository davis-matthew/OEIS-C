/*
 * A180639  a(n) is the number of integers k <= n that are not solution of
 *          phi(x) = k for any x, where phi is the Euler totient function
 *          A000010.
 * https://oeis.org/A180639
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 380, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = n − #{k ≤ n | ∃x: φ(x)=k}.
   The routine keeps a totient sieve up to 2·n+10, counts all
   values ≤ n that appear, and returns the complement. */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* offset starts at 1 */

    size_t N = (size_t)n;                   /* n fits in size_t (≤ 10000) */
    size_t limit = N * 2 + 10;              /* 2·n+10 is safely above every totient ≤ n */

    /* --------- compute φ values up to ‘limit’ --------- */
    uint64_t *phi = (uint64_t *)malloc((limit + 1) * sizeof(uint64_t));
    if (!phi) return (seq_t)0;              /* allocation failure → 0 */

    for (size_t i = 0; i <= limit; ++i) phi[i] = (uint64_t)i;
    for (size_t i = 2; i <= limit; ++i)
    {
        if (phi[i] == (uint64_t)i)          /* i is prime */
        {
            uint64_t p = (uint64_t)i;
            for (size_t j = i; j <= limit; j += i)
                phi[j] -= phi[j] / p;
        }
    }

    /* --------- mark which k ≤ n appear as a totient --------- */
    unsigned char *seen = (unsigned char *)calloc(N + 1, sizeof(unsigned char));
    if (!seen)
    {
        free(phi);
        return (seq_t)0;
    }

    for (size_t i = 1; i <= limit; ++i)
    {
        uint64_t val = phi[i];
        if (val <= N)
            seen[(size_t)val] = 1;
    }

    /* --------- count totients up to n --------- */
    size_t totient_cnt = 0;
    for (size_t k = 1; k <= N; ++k)
        if (seen[k]) ++totient_cnt;

    /* --------- cleanup --------- */
    free(phi);
    free(seen);

    /* --------- result: non‑totients count --------- */
    return (seq_t)(N - totient_cnt);
}
