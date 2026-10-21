/*
 * A063838  Numbers k such that mu(k) + mu(k+1) + mu(k+2) = 3.
 * https://oeis.org/A063838
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, changed
 * Formula:   a(n) = the n-th integer k such that μ(k)+μ(k+1)+μ(k+2)=3 (i.e.
 *            μ(k)=μ(k+1)=μ(k+2)=1). The implementation precomputes the Möbius
 *            function up to a fixed maximum (4,000,000) and records the
 *            positions where the three consecutive values equal 1. The table
 *            of such k is expanded lazily until it contains...
 * Range:     u8 n <= 7, u16 n <= 977, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* ---------- Precomputation constants ---------- */
#define MAX_LIMIT 4000000

/* ---------- Internal storage ---------- */
static int8_t  mu[MAX_LIMIT + 1];   /* Möbius values: -1, 0, 1 */
static int     lp[MAX_LIMIT + 1];   /* least prime factor */
static int     primes[400000];      /* primes up to MAX_LIMIT (≈2.8e5) */
static int     primes_cnt;

/* ------- Solutions table ---------- */
static idx_t  *solutions = NULL;    /* k values where μ(k)=μ(k+1)=μ(k+2)=1 */
static size_t  sol_cnt   = 0;
static size_t  sol_cap   = 0;

/* ------- Current search position ---------- */
static idx_t  next_k_to_check = 0;

/* ---------- Helper: expand solutions capacity ---------- */
static void grow_solutions(size_t desired)
{
    if (sol_cap >= desired) return;
    size_t new_cap = sol_cap ? sol_cap << 1 : 64;
    while (new_cap < desired) new_cap <<= 1;
    solutions = (idx_t *)realloc(solutions, new_cap * sizeof(idx_t));
    sol_cap = new_cap;
}

/* ---------- Möbius linear sieve up to MAX_LIMIT ---------- */
static void init_mobius(void)
{
    /* arrays are zero-initialised statically */
    mu[1] = 1;
    primes_cnt = 0;
    for (int i = 2; i <= MAX_LIMIT; ++i) {
        if (lp[i] == 0) {
            lp[i] = i;
            mu[i] = -1;
            primes[primes_cnt++] = i;
        }
        for (int j = 0; j < primes_cnt; ++j) {
            int p = primes[j];
            long long t = (long long)i * p;
            if (t > MAX_LIMIT) break;
            int ti = (int)t;
            lp[ti] = p;
            if (i % p == 0) {
                mu[ti] = 0;
                break;
            } else {
                mu[ti] = -mu[i];
            }
        }
    }
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n)
{
    static int initialized = 0;
    if (!initialized) {
        init_mobius();      /* precompute Möbius once */
        initialized = 1;
    }

    /* Fill the solutions table up to index n (1‑based) */
    while (sol_cnt < (size_t)n) {
        idx_t k = next_k_to_check;
        /* Ensure we have Möbius up to k+2 (always true in current pre‑computation) */
        if (k + 2 > MAX_LIMIT) {
            /* The pre‑computed limit is insufficient, but this should not happen
               for n up to about 10 000. */
            return (seq_t)0;
        }
        /* Check the condition μ(k)=μ(k+1)=μ(k+2)=1 */
        if (mu[k] == 1 && mu[k + 1] == 1 && mu[k + 2] == 1) {
            grow_solutions(sol_cnt + 1);
            solutions[sol_cnt++] = k;
            if (sol_cnt == (size_t)n) {
                next_k_to_check = k + 1;
                return (seq_t)solutions[n - 1];
            }
        }
        ++next_k_to_check;
    }

    /* Should never reach here */
    return (seq_t)solutions[n - 1];
}
