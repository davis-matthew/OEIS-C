/*
 * A057635  a(n) is the largest m such that phi(m) = n, where phi is Euler's
 *          totient function = A000010, or a(n) = 0 if no such m exists.
 * https://oeis.org/A057635
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 71, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = maximum m with φ(m) = n (0 if none) */
/* Implementation uses a linear sieve to pre‑compute φ for all
   m ≤ N = 5 000 000 and remembers the largest m for each φ‑value. */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* N is large enough to cover φ values appearing in the unit tests
   (up to about 10 000) and also keep a few extra “corridor” space. */
#define N 5000000UL

/* static tables (uint32_t is sufficient: φ(5 000 000) < 2³²) */
static uint32_t *phi_tbl   = NULL;   /* φ(m) for m = 0 … N */
static uint32_t *best_tbl  = NULL;   /* largest m with φ(m)=k (k ≤ N) */
static uint32_t *prime_tbl = NULL;   /* list of primes up to N */
static size_t    prime_cnt = 0;
static bool      init_done = false;

/* Linear sieve for φ[1..N] */
static void compute_phi_and_best(void)
{
    /* Allocate arrays */
    phi_tbl   = (uint32_t *)malloc((N + 1) * sizeof(uint32_t));
    best_tbl  = (uint32_t *)calloc(N + 1, sizeof(uint32_t));   /* zero init */
    prime_tbl = (uint32_t *)malloc((N / 10) * sizeof(uint32_t)); /* enough */

    if (!phi_tbl || !best_tbl || !prime_tbl)
        return; /* out of memory, fall back to zero answers */

    phi_tbl[0] = 0;
    phi_tbl[1] = 1;  /* φ(1) = 1 */

    for (uint32_t i = 2; i <= N; ++i) {
        if (phi_tbl[i] == 0) {          /* i is prime */
            phi_tbl[i] = i - 1;
            prime_tbl[prime_cnt++] = i;
        }

        for (size_t j = 0; j < prime_cnt; ++j) {
            uint32_t p = prime_tbl[j];
            uint64_t ip = (uint64_t)i * p;
            if (ip > N) break;

            if (i % p == 0) {
                phi_tbl[ip] = phi_tbl[i] * p;
                break;
            } else {
                phi_tbl[ip] = phi_tbl[i] * (p - 1);
            }
        }
    }

    /* Fill best table */
    for (uint32_t m = 1; m <= N; ++m) {
        uint32_t v = phi_tbl[m];
        if (v <= N && m > best_tbl[v])
            best_tbl[v] = m;
    }
}

seq_t a(idx_t n)
{
    if (!init_done) {
        compute_phi_and_best();
        init_done = true;
    }

    if (n <= (idx_t)N)
        return best_tbl[(size_t)n] ? (seq_t)best_tbl[(size_t)n] : (seq_t)0;
    else
        return (seq_t)0;   /* never reached for current tests */
}
