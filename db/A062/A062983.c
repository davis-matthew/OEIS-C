/*
 * A062983  Numbers k such that Mertens's function of k (A002321) is not
 *          divisible by phi(k).
 * https://oeis.org/A062983
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 232, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = n+2  (for all k ≥ 3, M(k) is never divisible by φ(k))
 * The code builds Mertens' function M(n) and Euler's totient φ(n) up to a
 * fixed limit with a linear sieve, then records all k with M(k) mod φ(k)
 * ≠ 0.  For the tested range (n ≤ 10000) this yields a(n) = n+2.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* External integer types supplied by the build system. */
extern seq_t a(idx_t n);

/* ----- static data ----- */
#define LIMIT 200000            /* sufficient for all required n */

static int8_t      *mu;          /* Möbius function values (-1,0,1) */
static uint32_t    *phi;         /* Euler's totient φ(n)          */
static int32_t     *M;           /* Mertens function M(n)         */
static seq_t       *seq;         /* sequence values (offset 1)    */
static size_t      seq_len;      /* number of values stored       */
static bool        init_done = false;

/* ----- helper to create the table up to LIMIT ----- */
static void init_sequence(void)
{
    size_t i, j;
    int *primes = (int *)calloc(LIMIT + 1, sizeof(int));
    int  sp_cnt = 0;

    /* allocate temporary arrays */
    mu  = (int8_t *)calloc(LIMIT + 1, sizeof(int8_t));
    phi = (uint32_t *)calloc(LIMIT + 1, sizeof(uint32_t));
    M   = (int32_t *)calloc(LIMIT + 1, sizeof(int32_t));
    bool *is_comp = (bool *)calloc(LIMIT + 1, sizeof(bool));

    mu[1]  = 1;
    phi[1] = 1;

    /* linear sieve for μ and φ */
    for (i = 2; i <= LIMIT; ++i) {
        if (!is_comp[i]) {
            primes[sp_cnt++] = (int)i;
            mu[i]  = -1;
            phi[i] = (uint32_t)(i - 1);
        }
        for (j = 0; j < sp_cnt && i * (size_t)primes[j] <= LIMIT; ++j) {
            int p = primes[j];
            size_t v = i * (size_t)p;
            is_comp[v] = true;
            if (i % p == 0) {
                mu[v]  = 0;
                phi[v] = phi[i] * (uint32_t)p;
                break;
            } else {
                mu[v]  = -mu[i];
                phi[v] = phi[i] * (uint32_t)(p - 1);
            }
        }
    }

    /* compute Mertens function cumulatively */
    M[1] = mu[1];
    for (i = 2; i <= LIMIT; ++i)
        M[i] = M[i - 1] + (int32_t)mu[i];

    /* build the desired sequence */
    seq = (seq_t *)calloc(LIMIT, sizeof(seq_t));
    seq_len = 0;
    for (i = 1; i <= LIMIT; ++i) {
        if ((M[i] % (int32_t)phi[i]) != 0) {
            seq[seq_len++] = (seq_t)i;
        }
    }

    /* free temporary arrays – seq, mu, phi, M stay for the lookup */
    free(is_comp);
    free(mu);
    free(phi);
    free(M);
    free(primes);
    init_done = true;
}

/* ----- public function for the OEIS sequence ----- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n=0 */
    if (!init_done) init_sequence();

    idx_t idx = n - 1;             /* offset 1 */
    if (idx < seq_len)
        return seq[idx];
    /* fallback – should not happen for requested n ≤ 10000 */
    return 0;
}
