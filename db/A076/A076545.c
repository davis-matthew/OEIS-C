/*
 * A076545  a(n) = Sum_{k=1..n} (mu(k) + sqf(k)) where mu(k) is Moebius
 *          function, sqf(k)=1 if k is squarefree and sqf(k)=-1 otherwise.
 * https://oeis.org/A076545
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=1..n} (2*mu(k)^2 + mu(k) - 1)
 * Range:     u8 n <= 1198, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where mu(k) is the Möbius function. */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

/* --------- static state for memoisation ---------- */
static idx_t            max_computed = 0;   /* highest n that is ready */

static int8_t *         mu_vals     = NULL; /* μ values, signed 8‑bit */
static uint8_t *        comp_flags  = NULL; /* composite flags */
static seq_t *          sums        = NULL; /* accumulated sums */

static size_t           mu_cap      = 0;
static size_t           comp_cap    = 0;
static size_t           sum_cap     = 0;

static uint32_t *       primes      = NULL; /* list of primes (up to current n) */
static size_t           prime_cnt   = 0;
static size_t           prime_cap   = 0;

/* --------- helper for dynamic arrays ---------- */
static void ensure_cap(void **ptr, size_t *cap, size_t needed, size_t elem_size)
{
    if (*cap >= needed)
        return;
    size_t old = *cap;
    size_t new_cap = old ? old << 1 : 16;
    while (new_cap < needed)
        new_cap <<= 1;
    void *tmp = realloc(*ptr, new_cap * elem_size);
    *ptr = tmp;
    *cap = new_cap;
    /* initialise new part to zero */
    if (new_cap > old)
        memset((char *)tmp + old * elem_size, 0, (new_cap - old) * elem_size);
}

/* --------- build tables up to n ---------- */
static void ensure_up_to(idx_t n)
{
    if (n <= max_computed)
        return;

    /* enlarge arrays to hold indices 0..n */
    size_t needed = (size_t)(n + 1);
    ensure_cap((void**)&mu_vals,   &mu_cap,   needed, sizeof(int8_t));
    ensure_cap((void**)&comp_flags,&comp_cap, needed, sizeof(uint8_t));
    ensure_cap((void**)&sums,      &sum_cap,  needed, sizeof(seq_t));
    ensure_cap((void**)&primes,    &prime_cap,needed, sizeof(uint32_t));

    /* initialise μ[1] and zero others (μ[0] unused) */
    mu_vals[0] = 0;
    mu_vals[1] = 1;
    comp_flags[0] = 0;
    comp_flags[1] = 0;
    sums[0] = 0;

    /* Initial mu for i>=2 is 1 (will be flipped later) */
    for (idx_t i = 2; i <= n; ++i) {
        mu_vals[i]  = 1;
        comp_flags[i] = 0;
    }

    /* Sieve of Eratosthenes + Möbius computation */
    prime_cnt = 0;
    for (idx_t i = 2; i <= n; ++i) {
        if (!comp_flags[i]) {          /* i is prime */
            primes[prime_cnt++] = (uint32_t)i;

            /* mark multiples composite */
            for (idx_t j = i + i; j <= n; j += i)
                comp_flags[j] = 1;

            /* flip sign of multiples (prime factor counted once) */
            for (idx_t j = i; j <= n; j += i)
                mu_vals[j] = -mu_vals[j];

            /* multiples of p² get μ = 0 */
            if (i <= n / i) {          /* avoid overflow */
                idx_t square = i * i;
                for (idx_t j = square; j <= n; j += square)
                    mu_vals[j] = 0;
            }
        }
    }

    /* Compute cumulative sums a(k) */
    seq_t accum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        int8_t mk = mu_vals[k];
        seq_t term = (seq_t)(2LL * mk * mk + mk - 1); /* term fits in signed */
        accum += term;
        sums[k] = accum;
    }

    max_computed = n;
}

/* --------- public function ---------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* outside the official offset */
    ensure_up_to(n);
    return sums[n];
}
