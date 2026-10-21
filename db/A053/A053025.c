/*
 * A053025  Number of iterations of the number of divisors function (A000005)
 *          required to reach a fixed point (1 or 2) when started at n!.
 * https://oeis.org/A053025
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of terms (including the starting n!) in the *
 *            iteration chain of the divisor‑count function τ beginning with
 *            n! * until a fixed point 1 or 2 is reached
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* --------------------------------------------------------------------
 *  Prime table (sieve) – enough for n up to about 200 000.
 * -------------------------------------------------------------------- */
#define MAX_PRIMES 20000
static idx_t primes[MAX_PRIMES];
static size_t prime_cnt = 0;
static bool primes_computed = false;

static void compute_primes(void)
{
    if (primes_computed)
        return;
    const int LIMIT = 200000;
    char *is_comp = (char *)malloc(LIMIT + 1);
    if (!is_comp)
        return;                      /* should never happen in tests */
    memset(is_comp, 0, LIMIT + 1);
    for (int i = 2; i * i <= LIMIT; ++i)
        if (!is_comp[i])
            for (int j = i * i; j <= LIMIT; j += i)
                is_comp[j] = 1;
    prime_cnt = 0;
    for (int i = 2; i <= LIMIT; ++i)
        if (!is_comp[i]) {
            if (prime_cnt < MAX_PRIMES)
                primes[prime_cnt++] = (idx_t)i;
        }
    free(is_comp);
    primes_computed = true;
}

/* --------------------------------------------------------------------
 *  Small factorisation structure.
 * -------------------------------------------------------------------- */
#define MAX_FACTS 2000
typedef struct {
    int    cnt;
    idx_t primes[MAX_FACTS];
    idx_t exps [MAX_FACTS];
} Factor;

/* add a prime factor, merging if already present */
static void add_prime(Factor *f, idx_t p, idx_t e)
{
    if (e == 0)
        return;
    for (int i = 0; i < f->cnt; ++i)
        if (f->primes[i] == p) {
            f->exps[i] += e;
            return;
        }
    if (f->cnt < MAX_FACTS) {
        f->primes[f->cnt] = p;
        f->exps [f->cnt] = e;
        ++f->cnt;
    }
}

/* merge factorisations: f += g */
static void merge_factor(Factor *f, const Factor *g)
{
    for (int i = 0; i < g->cnt; ++i)
        add_prime(f, g->primes[i], g->exps[i]);
}

/* factorise a small integer (≤ value of an exponent+1) */
static void factor_small(idx_t val, Factor *out)
{
    out->cnt = 0;
    idx_t x = val;
    for (size_t i = 0; i < prime_cnt && primes[i] * primes[i] <= x; ++i) {
        idx_t p = primes[i];
        if (x % p == 0) {
            idx_t e = 0;
            while (x % p == 0) {
                x /= p;
                ++e;
            }
            add_prime(out, p, e);
        }
    }
    if (x > 1)
        add_prime(out, x, 1);
}

/* factorial prime exponentisation using Legendre */
static void factorial_factorisation(idx_t n, Factor *out)
{
    out->cnt = 0;
    for (size_t i = 0; i < prime_cnt && primes[i] <= n; ++i) {
        idx_t p   = primes[i];
        idx_t exp = 0;
        idx_t m   = n;
        while (m) {
            m   /= p;
            exp += m;
        }
        add_prime(out, p, exp);
    }
}

/* τ(N) factorisation – product of (e+1) for each prime factor of N */
static void tau_factorisation(const Factor *F, Factor *T)
{
    T->cnt = 0;
    for (int i = 0; i < F->cnt; ++i) {
        idx_t val = F->exps[i] + 1;
        Factor part;
        factor_small(val, &part);
        merge_factor(T, &part);
    }
}

/* --------------------------------------------------------------------
 *  The required OEIS sequence function.
 * -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    compute_primes();

    if (n <= 2)                /* 1! = 1, 2! = 2 – fixed points themselves */
        return (seq_t)1;

    idx_t  count = 1;          /* include starting n! */
    Factor cur, next;

    factorial_factorisation(n, &cur);

    for (;;) {
        tau_factorisation(&cur, &next);
        ++count;

        /* stop when next == 1 or 2 */
        if (next.cnt == 0)                /* 1 */
            break;
        if (next.cnt == 1 && next.primes[0] == 2 && next.exps[0] == 1) /* 2 */
            break;

        cur = next;                      /* proceed to next iteration */
    }
    return (seq_t)count;
}
