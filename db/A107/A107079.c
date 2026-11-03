/*
 * A107079  Minimal number of squared primes in a squarefree gap of length n.
 * https://oeis.org/A107079
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + number of squarefree integers in [1, n-1] (mu(0) is
 *            treated as 1, so a(1)=1). This follows from the definition a(n)
 *            = Σ_{k=0}^{n-1} (mu(n‑k‑1) mod 2)
 * Range:     u8 n <= 418, u16 n <= 100000, u32 n <= 100000, u64 n <= 100000,
 *            u128 n <= 100000, big any n
 * Validated: standard, pass, 90 terms through n = 9153 (2026-11-03)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A107079: a(n) = 1 + #{1 <= i < n : i squarefree}. Squarefree numbers come
 * from a sieve by p^2 for every p (the original only tried p <= 97, so it
 * counted multiples of 101^2 and larger squares from n = 10202 on); the table
 * of counts is kept and grows as needed. */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint32_t *cnt = NULL;              /* cnt[m] = #squarefree i < m */
static uint64_t cnt_len = 0;

static void build(uint64_t N)
{
    unsigned char *sf = (unsigned char *)malloc((size_t)N + 1);
    uint32_t *c = (uint32_t *)malloc(((size_t)N + 1) * sizeof *c);
    if (!sf || !c) abort();
    memset(sf, 1, (size_t)N + 1);
    for (uint64_t p = 2; p * p <= N; ++p)
        for (uint64_t m = p * p; m <= N; m += p * p) sf[m] = 0;
    c[0] = c[1] = 0;
    for (uint64_t i = 1; i < N; ++i) c[i + 1] = c[i] + sf[i];
    free(sf);
    free(cnt);
    cnt = c;
    cnt_len = N + 1;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    if ((uint64_t)n >= cnt_len) {
        uint64_t N = cnt_len ? 2 * (cnt_len - 1) : 1 << 16;
        while (N < (uint64_t)n) N *= 2;
        build(N);
    }
    return (seq_t)(1 + cnt[n]);
}
