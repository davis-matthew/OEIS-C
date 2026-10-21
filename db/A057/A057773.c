/*
 * A057773  a(n) = Sum_{i=1..n} nu_2(prime(i) - 1) where nu_2(m) = exponent of
 *          highest power of 2 dividing m.
 * https://oeis.org/A057773
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=1..n} ν₂(prime_i – 1)
 * Range:     u8 n <= 134, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (ν₂(m) is the exponent of the highest power of 2 dividing m) */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/* Count trailing zeros of a 64‑bit word – i.e. ν₂(x) for x > 0 */
static inline unsigned int v2_uint64(uint64_t x)
{
    unsigned int c = 0;
    while ((x & 1ULL) == 0 && x != 0) {
        x >>= 1;
        ++c;
    }
    return c;
}

/* Grow a dense array of seq_t */
static void ensure_seq_cap(seq_t **arr, size_t *cap, size_t need)
{
    if (need <= *cap) return;
    size_t newcap = *cap ? *cap : 8;
    while (newcap < need) newcap <<= 1;
    *arr = (seq_t *)realloc(*arr, newcap * sizeof(seq_t));
    *cap = newcap;
}

/* Grow a dense array of uint32_t */
static void ensure_uint32_cap(uint32_t **arr, size_t *cap, size_t need)
{
    if (need <= *cap) return;
    size_t newcap = *cap ? *cap : 8;
    while (newcap < need) newcap <<= 1;
    *arr = (uint32_t *)realloc(*arr, newcap * sizeof(uint32_t));
    *cap = newcap;
}

/* ---------- main sequence function ------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* values below the offset are irrelevant */

    /* static state                                         */
    static uint32_t *primes   = NULL;          /* list of primes found so far          */
    static size_t   primes_cnt = 0;            /* number of primes stored              */
    static size_t   primes_cap = 0;            /* allocated capacity of primes array   */
    static seq_t   *a_cache   = NULL;          /* a_cache[1] = a(1), a_cache[2] = a(2), … */
    static size_t   a_cap     = 0;
    static uint64_t candidate = 3;             /* next odd candidate to test (after 2) */

    /* initialise caches on first call                          */
    if (a_cache == NULL) {
        ensure_seq_cap(&a_cache, &a_cap, 2);
        a_cache[0] = 0;                        /* unused placeholder                 */
        a_cache[1] = 0;                        /* a(1) = 0                            */
    }
    if (primes == NULL) {
        ensure_uint32_cap(&primes, &primes_cap, 1);
    }

    /* generate primes and accumulate the sequence until we have n primes   */
    while (primes_cnt < (size_t)n) {
        uint64_t p;

        /* the first prime is 2, handled only once                         */
        if (primes_cnt == 0) {
            p = 2;
        } else {
            /* find the next prime candidate                                   */
            bool is_prime = true;
            while (true) {
                uint64_t sq = (uint64_t)candidate * (uint64_t)candidate;
                for (size_t i = 0; i < primes_cnt; ++i) {
                    uint32_t pr = primes[i];
                    if (sq <= (uint64_t)pr * (uint64_t)pr) break;
                    if (candidate % pr == 0) {
                        is_prime = false;
                        break;
                    }
                }
                if (is_prime) break;
                candidate += 2;                 /* skip even numbers after 2      */
                is_prime = true;
            }
            p = candidate;
            candidate += 2;                     /* next odd candidate             */
        }

        /* store the new prime                                         */
        ensure_uint32_cap(&primes, &primes_cap, primes_cnt + 1);
        primes[primes_cnt] = (uint32_t)p;
        ++primes_cnt;

        /* compute ν₂(p – 1)                                           */
        unsigned int v = v2_uint64(p - 1);

        /* extend a_cache                                                     */
        ensure_seq_cap(&a_cache, &a_cap, primes_cnt + 1);
        if (primes_cnt == 1) {                  /* a(1) = 0 already set            */
            /* no addition needed – stored earlier as 0                */
        } else {
            a_cache[primes_cnt] = a_cache[primes_cnt - 1] + (seq_t)v;
        }
    }

    return a_cache[n];
}
