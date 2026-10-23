/*
 * A030232  Numbers k such that the number of divisors of k! divides the
 *          number of divisors of (2k)!.
 * https://oeis.org/A030232
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 121, u32 n <= 121, u64 n <= 121,
 *            u128 n <= 121, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of A030232: numbers k such that d(k!) divides d((2k)!).
 * The algorithm maintains exponent tables for k! and (2k)!,
 * updates them incrementally and keeps the prime exponent tables
 * of the divisor counts.  Divisibility is tested by comparing the
 * prime exponents of the two divisor counts.  All data are stored
 * in static tables that are allocated on first use, allowing
 * non‑monotonic calls to a(idx_t).  The routine works for
 * 64‑bit seq_t as well as for arbitrary‑precision seq_t because all
 * internal arithmetic uses only integer types.  The limit used for
 * precomputation (400 000) is sufficient for n up to about 10 000.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define LIMIT 400000   /* sieve limit; covers 2*k for k up to ~200k */

/* prime factor tables --------------------------------------------- */
static int      *spf          = NULL;  /* smallest prime factor */
static int      *prime_idx    = NULL;  /* prime → index in prime list */
static uint32_t *primes       = NULL;  /* list of primes */
static size_t    num_primes   = 0;      /* number of primes */

/* exponent tables for factorials ----------------------------------- */
static int  *exp_fact_k  = NULL;   /* exponents of primes in k!   */
static int  *exp_fact_2k = NULL;   /* exponents of primes in (2k)! */

/* prime exponent tables for divisor counts ------------------------ */
static long long *tau_exp1 = NULL;   /* exponents of primes in d(k!)   */
static long long *tau_exp2 = NULL;   /* exponents of primes in d((2k)!) */

/* global state ----------------------------------------------------- */
static idx_t          current_k = 0;  /* largest k processed so far */
static seq_t         *seq       = NULL;   /* stored sequence values    */
static size_t         seq_len   = 0;      /* number of values stored  */
static size_t         seq_cap   = 0;      /* allocated capacity       */

/* --------------------------------------------------------------- */
static void _init_once(void);

/* update the factorisation tables for an integer n */
static void factor_and_update(int n, int *exp_fact, long long *tau_exps);

/* factor a small integer value and add/subtract its prime
 * factor exponents to the provided table. */
static void factor_update_value(int val, long long *table, int delta);

/* increment the global state by one k */
static void _increment_k(idx_t k);

/* ensure the sequence array has at least n elements */
static void _ensure_seq(idx_t n);

/* --------------------------------------- */
static void _init_once(void)
{
    if (spf != NULL) return;   /* already initialised */

    /* smallest prime factor sieve */
    spf = (int *)malloc((LIMIT + 1) * sizeof(int));
    prime_idx = (int *)malloc((LIMIT + 1) * sizeof(int));
    for (int i = 0; i <= LIMIT; ++i) {
        spf[i] = i;
        prime_idx[i] = -1;
    }
    for (int i = 2; i * i <= LIMIT; ++i) if (spf[i] == i) {
        for (int j = i * i; j <= LIMIT; j += i)
            if (spf[j] == j) spf[j] = i;
    }

    /* build prime list and inverse map */
    size_t prm_cap = LIMIT / 10 + 10;
    primes = (uint32_t *)malloc(prm_cap * sizeof(uint32_t));
    for (int i = 2; i <= LIMIT; ++i)
        if (spf[i] == i) {
            if (num_primes == prm_cap) {
                prm_cap <<= 1;
                primes = (uint32_t *)realloc(primes, prm_cap * sizeof(uint32_t));
            }
            primes[num_primes] = (uint32_t)i;
            prime_idx[i] = (int)num_primes;
            ++num_primes;
        }

    /* allocate exponent tables */
    exp_fact_k  = (int  *)calloc(num_primes, sizeof(int));
    exp_fact_2k = (int  *)calloc(num_primes, sizeof(int));
    tau_exp1    = (long long *)calloc(num_primes, sizeof(long long));
    tau_exp2    = (long long *)calloc(num_primes, sizeof(long long));
}

/* --------------------------------------------------------------- */
static void factor_update_value(int val, long long *table, int delta)
{
    while (val > 1) {
        int p = spf[val];
        int cnt = 0;
        while (val % p == 0) { val /= p; ++cnt; }
        int idx = prime_idx[p];
        table[idx] += (long long)delta * cnt;
    }
}

/* --------------------------------------------------------------- */
static void factor_and_update(int n, int *exp_fact, long long *tau_exps)
{
    while (n > 1) {
        int p = spf[n];
        int cnt = 0;
        while (n % p == 0) { n /= p; ++cnt; }
        int idx = prime_idx[p];
        int old = exp_fact[idx];
        int new_val = old + cnt;
        exp_fact[idx] = new_val;

        if (old + 1 > 1) factor_update_value(old + 1, tau_exps, -1);
        if (new_val + 1 > 1) factor_update_value(new_val + 1, tau_exps, +1);
    }
}

/* --------------------------------------------------------------- */
static void _increment_k(idx_t k)
{
    /* update k! part */
    factor_and_update((int)k, exp_fact_k, tau_exp1);

    /* update (2k)! part */
    int a = (int)(2 * k - 1);   /* may be 1; skip if so */
    if (a > 1) factor_and_update(a, exp_fact_2k, tau_exp2);
    int b = (int)(2 * k);
    factor_and_update(b, exp_fact_2k, tau_exp2);
}

/* --------------------------------------------------------------- */
static void _ensure_seq(idx_t n)
{
    _init_once();

    while (seq_len < n) {
        ++current_k;
        _increment_k(current_k);

        /* test d(k!) | d((2k)!) */
        bool good = true;
        for (size_t i = 0; i < num_primes; ++i) {
            if (tau_exp1[i] > tau_exp2[i]) { good = false; break; }
        }

        if (good) {
            if (seq_len == seq_cap) {
                seq_cap = (seq_cap == 0) ? 64 : seq_cap * 2;
                seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
            }
            seq[seq_len++] = (seq_t)current_k;
        }
    }
}

/* --------------------------------------- */
seq_t a(idx_t n)
{
    _ensure_seq(n);
    return seq[n - 1];
}
