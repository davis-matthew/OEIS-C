/*
 * A073776  a(n) = Sum_{k=1..n} -mu(k+1) * a(n-k), with a(0)=1.
 * https://oeis.org/A073776
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 21, u32 n <= 41, u64 n <= 82, u128 n <= 163,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1
   a(n) = Σ_{k=1..n} -mu(k+1) * a(n-k)
   where mu is the Möbius function.  */  

#include <stdlib.h>
#include <stdint.h>

/* ---------- Möbius value table --------------------------------- */
static int8_t *mu_cache = NULL;     /* mu_cache[i] = mu(i)   */
static idx_t   mu_max   = 0;        /* largest index computed */

/* Build a linear‑sieve Möbius table up to n (inclusive). */
static void
ensure_mu_up_to(idx_t n)
{
    if (n <= mu_max) return;

    /* reallocate a fresh table */
    int8_t *new_mu = (int8_t *)calloc((size_t)(n + 1), sizeof(int8_t));
    if (!new_mu) return;
    if (mu_cache) free(mu_cache);
    mu_cache = new_mu;
    mu_max   = n;

    /* Möbius of 1 */
    if (n >= 1) mu_cache[1] = 1;

    /* linear sieve */
    uint8_t *is_comp = (uint8_t *)calloc((size_t)(n + 1), 1);
    if (!is_comp) return;

    /* ample prime storage; n+1 guarantees no overflow */
    size_t primes_cap = (size_t)n + 1;
    uint64_t *primes  = (uint64_t *)malloc(primes_cap * sizeof(uint64_t));
    if (!primes) { free(is_comp); return; }

    size_t primes_sz = 0;
    for (idx_t i = 2; i <= n; ++i) {
        if (!is_comp[i]) {                     /* i is prime */
            if (primes_sz < primes_cap) {
                primes[primes_sz++] = (uint64_t)i;
                mu_cache[i] = -1;               /* μ(p) = -1 */
            }
        }
        for (size_t j = 0; j < primes_sz; ++j) {
            uint64_t p = primes[j];
            idx_t ip   = i * p;
            if (ip > n) break;
            is_comp[ip] = 1;
            if (i % p == 0) {                  /* square factor */
                mu_cache[ip] = 0;
                break;
            } else {
                mu_cache[ip] = (int8_t)(-mu_cache[i]);   /* μ(ip)= -μ(i) */
            }
        }
    }

    free(is_comp);
    free(primes);
}

/* ---------- Sequence cache ------------------------------------- */
static seq_t *a_cache = NULL;
static size_t a_cap   = 0;   /* array capacity (in elements) */
static size_t a_len   = 0;   /* number of values stored */

/* Ensure that values a[0..n] are computed. */
static void
ensure_a_size(idx_t n)
{
    if ((size_t)n + 1 <= a_len) return;

    /* need Möbius values up to (n+1) because of k+1 in the recurrence */
    ensure_mu_up_to(n + 1);

    /* grow the cache if necessary */
    if ((size_t)n + 1 > a_cap) {
        size_t new_cap = a_cap > 0 ? (a_cap << 1) : 1;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        if (!tmp) return;
        a_cache = tmp;
        a_cap   = new_cap;
    }

    /* compute missing terms */
    for (idx_t idx = a_len; idx <= n; ++idx) {
        if (idx == 0) {
            a_cache[0] = 1;
        } else {
            seq_t pos = 0;
            seq_t neg = 0;
            for (idx_t k = 1; k <= idx; ++k) {
                int8_t mu = mu_cache[k + 1];
                if (mu == -1) {
                    pos += a_cache[idx - k];
                } else if (mu == 1) {
                    neg += a_cache[idx - k];
                }
            }
            a_cache[idx] = pos - neg;
        }
    }
    a_len = n + 1;
}

/* ---------- Public interface ----------------------------------- */
seq_t a(idx_t n)
{
    ensure_a_size(n);
    return a_cache[n];
}
