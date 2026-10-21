/*
 * A224222  a(0)=3; for n>0, a(n) is the smallest prime q not already in the
 *          sequence such that the n-th prime p(n) divides a(n-1)+q. If no
 *          such prime q exists, the sequence terminates.
 * https://oeis.org/A224222
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(0)=3; for n>0, a(n) is the smallest prime q not already in *
 *            the sequence such that the n‑th prime p(n) divides a(n‑1)+q. If
 *            no * such prime q exists the sequence terminates
 * Range:     u8 n <= 17, u16 n <= 19, u32 n <= 19, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>

#include "../utils/is_prime_small.h"

/* ---------- static state ---------- */
static seq_t *seq_cache   = NULL;   /* computed terms          */
static size_t seq_len     = 0;      /* number of terms stored  */
static size_t seq_cap     = 0;      /* allocated capacity      */

static uint64_t *primes    = NULL;    /* list of primes          */
static size_t    prime_cnt  = 0;      /* number of primes stored */
static size_t    prime_cap  = 0;      /* allocated capacity      */

static seq_t *used_primes  = NULL;    /* primes that already appear in the sequence */
static size_t used_len     = 0;
static size_t used_cap     = 0;

static bool terminated = false;       /* true once the sequence cannot continue */

/* ---------- helper functions ---------- */

static void ensure_cache_cap(size_t need)
{
    if (need <= seq_cap) return;
    size_t newcap = seq_cap ? seq_cap : 16;
    while (newcap < need) newcap <<= 1;
    seq_cache = (seq_t *)realloc(seq_cache, newcap * sizeof(seq_t));
    seq_cap = newcap;
}

static void ensure_prime_cap(size_t need)
{
    if (need <= prime_cap) return;
    size_t newcap = prime_cap ? prime_cap : 16;
    while (newcap < need) newcap <<= 1;
    primes = (uint64_t *)realloc(primes, newcap * sizeof(uint64_t));
    prime_cap = newcap;
}

static void ensure_used_cap(size_t need)
{
    if (need <= used_cap) return;
    size_t newcap = used_cap ? used_cap : 16;
    while (newcap < need) newcap <<= 1;
    used_primes = (seq_t *)realloc(used_primes, newcap * sizeof(seq_t));
    used_cap = newcap;
}

/* add a prime to the used set */
static void add_used_prime(seq_t q)
{
    ensure_used_cap(used_len + 1);
    used_primes[used_len++] = q;
}

/* test whether a prime is already used */
static bool is_used_prime(seq_t q)
{
    for (size_t i = 0; i < used_len; ++i)
        if (used_primes[i] == q) return true;
    return false;
}

/* generate the next prime after the current last one */
static void generate_next_prime(void)
{
    uint64_t candidate = (prime_cnt == 0) ? 2U : primes[prime_cnt - 1] + 1U;
    while (!is_prime_small((seq_t)candidate)) candidate++;
    ensure_prime_cap(prime_cnt + 1);
    primes[prime_cnt++] = candidate;
}

/* return the (idx)-th prime (1‑based) */
static uint64_t prime_at(size_t idx)
{
    while (prime_cnt <= idx)
        generate_next_prime();
    return primes[idx - 1];               /* primes[0] = 2 -> p(1) */
}

/* ---------- main function ---------- */
seq_t a(idx_t n)
{
    if (terminated && n >= seq_len) return 0;

    /* ensure we have at least n+1 terms cached */
    if (n >= seq_len)
        ensure_cache_cap(n + 1);

    /* compute missing terms up to n */
    for (size_t m = seq_len; m <= (size_t)n; ++m)
    {
        if (terminated) { seq_cache[m] = 0; continue; }

        if (m == 0)
        {
            seq_cache[0] = (seq_t)3;
            add_used_prime(seq_cache[0]);
            continue;
        }

        /* p(n) : n-th prime (1‑based) */
        uint64_t p = prime_at(m);              /* m>=1 -> p(m) */

        /* compute remainder r where q ≡ -a(m-1) (mod p) */
        uint64_t rem = (uint64_t)(seq_cache[m-1] % p);
        uint64_t r = (p - rem) % p;            /* 0 <= r < p */

        seq_t chosen = 0;

        if (r == 0U)          /* only possible q is p itself */
        {
            if (is_used_prime(p))
            {           /* cannot choose any unused prime */
                terminated = true;
                chosen = 0;
            }
            else
                chosen = (seq_t)p;
        }
        else
        {
            uint64_t cand = r;
            if (cand < 2U)                     /* start at least 2 */
            {
                uint64_t off = 2U - cand;
                uint64_t inc = (off + p - 1U) / p;
                cand += inc * p;
            }

            /* search upward in arithmetic progression */
            while (1)
            {
                if (cand >= (uint64_t)UINT64_MAX) /* safety guard */
                {
                    terminated = true;
                    chosen = 0;
                    break;
                }

                if (is_prime_small((seq_t)cand) && !is_used_prime(cand))
                {
                    chosen = (seq_t)cand;
                    break;
                }
                cand += p;
            }
        }

        seq_cache[m] = chosen;
        if (chosen != 0) add_used_prime(chosen);
    }

    seq_len = (size_t)n + 1;
    return seq_cache[n];
}
