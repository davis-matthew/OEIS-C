/*
 * A083371  Primes p such that q-p >= 8, where q is the next prime after p.
 * https://oeis.org/A083371
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 1500, u32 n <= 1500, u64 n <= 1500,
 *            u128 n <= 1500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of A083371: primes p such that the next prime q satisfies
 * q − p ≥ 8. The routine dynamically generates primes and caches each
 * qualifying prime; subsequent calls retrieve results in O(1). */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* ------------------------------------------------------------ */
/*  Prime storage – list of currently known primes              */
/* ------------------------------------------------------------ */
static uint64_t *prime_list = NULL;   /* array of primes     */
static size_t   prime_cap   = 0;      /* capacity of array   */
static size_t   prime_len   = 0;      /* number of primes    */
static uint64_t last_prime  = 2;      /* last prime generated*/

static void add_prime(uint64_t p) {
    if (prime_len == prime_cap) {
        size_t new_cap = prime_cap ? prime_cap << 1 : 16;
        uint64_t *tmp   = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
        prime_list = tmp;
        prime_cap  = new_cap;
    }
    prime_list[prime_len++] = p;
}

/* Return the next prime after last_prime; update last_prime.   */
static uint64_t next_prime(void) {
    uint64_t cand = last_prime + 1;
    if ((cand & 1u) == 0) cand++;   /* skip even numbers */

    for (;;) {
        bool isp = true;
        for (size_t i = 0; i < prime_len; ++i) {
            uint64_t p = prime_list[i];
            if (p > cand / p) break;          /* p*p > cand */
            if (cand % p == 0) { isp = false; break; }
        }
        if (isp) {
            last_prime = cand;
            add_prime(cand);
            return cand;
        }
        cand += 2;                 /* next odd candidate */
    }
}

/* ------------------------------------------------------------ */
/*  Cache of qualifying primes (the sequence terms)            */
/* ------------------------------------------------------------ */
static seq_t *cache    = NULL;   /* array of terms   */
static size_t cache_cap = 0;     /* capacity of array*/
static size_t cache_len = 0;     /* number of terms   */

/* Ensure prime_list and cache are initialised.                */
static void init(void) {
    static bool ready = false;
    if (!ready) {
        /* primes */
        prime_cap   = 16;
        prime_list  = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
        add_prime(2);          /* start with 2 */

        /* sequence cache */
        cache_cap = 16;
        cache     = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        ready     = true;
    }
}

/* ------------------------------------------------------------ */
/*  Main sequence function                                     */
/* ------------------------------------------------------------ */
seq_t a(idx_t n) {
    init();

    if (n == 0) return 0;     /* out‑of‑range safe guard */

    /* Generate terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        uint64_t prev = last_prime;          /* current prime before next */
        uint64_t next = next_prime();        /* next prime after prev    */
        if (next - prev >= 8) {              /* prev qualifies */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap << 1 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)prev;
        }
    }
    return cache[(size_t)n - 1];
}
