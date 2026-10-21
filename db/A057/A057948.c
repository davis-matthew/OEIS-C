/*
 * A057948  S-primes: let S = {1,5,9, ... 4i+1, ...}; then an S-prime is in S
 *          but is not divisible by any members of S except itself and 1.
 * https://oeis.org/A057948
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth "S‑prime" of the set S = {1,5,9,…,4k+1}; An S‑prime
 *            is a member of S that has no non‑trivial divisor in S;
 *            equivalently a number ≡ 1 (mod 4) that is either – a prime ≡ 1
 *            (mod 4) or – the product of exactly two primes ≡ 3 (mod 4)
 *            (prime can repeat)
 * Range:     u8 none, u16 n <= 6971, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/*--- Static state -------------------------------------------------------*/
static uint8_t  *sieve_buf   = NULL;    /* sieve marks 0 = prime, 1 = composite   */
static uint64_t  sieve_limit = 0;       /* highest number sieved                 */
static uint32_t *prime_list  = NULL;    /* primes up to sieve_limit              */
static size_t    prime_cnt   = 0, prime_cap = 0;

static seq_t *sp_list   = NULL;         /* sorted list of S‑primes               */
static size_t sp_cnt = 0, sp_cap = 0;

/*--- Utility: compare for qsort ----------------------------------------*/
static int cmp_seq(const void *a, const void *b)
{
    seq_t va = *(const seq_t *)a;
    seq_t vb = *(const seq_t *)b;
    if (va < vb) return -1;
    if (va > vb) return  1;
    return 0;
}

/*--- Dynamic array helpers --------------------------------------------*/
static void ensure_prime_cap(size_t need)
{
    if (prime_cap >= need) return;
    size_t new_cap = prime_cap ? prime_cap * 2 : 64;
    while (new_cap < need) new_cap <<= 1;
    uint32_t *tmp = (uint32_t *)realloc(prime_list, new_cap * sizeof(uint32_t));
    prime_list = tmp;
    prime_cap   = new_cap;
}

static void ensure_sp_cap(size_t need)
{
    if (sp_cap >= need) return;
    size_t new_cap = sp_cap ? sp_cap * 2 : 64;
    while (new_cap < need) new_cap <<= 1;
    sp_list = (seq_t *)realloc(sp_list, new_cap * sizeof(seq_t));
    sp_cap   = new_cap;
}

/*--- Sieve of Eratosthenes ---------------------------------------------*/
static void sieve_up_to(uint64_t limit)
{
    if (limit <= sieve_limit) return;        /* already sufficient */

    free(sieve_buf);  sieve_buf   = NULL;
    free(prime_list); prime_list  = NULL;
    prime_cnt = 0;
    prime_cap = 0;

    sieve_buf = (uint8_t *)calloc(limit + 1, 1);
    if (!sieve_buf) return;                 /* allocation failure */

    for (uint64_t i = 2; i <= limit; ++i)
        if (!sieve_buf[i]) {
            ensure_prime_cap(prime_cnt + 1);
            prime_list[prime_cnt++] = (uint32_t)i;
            if (i * i <= limit)
                for (uint64_t j = i * i; j <= limit; j += i)
                    sieve_buf[(size_t)j] = 1U;
        }

    sieve_limit = limit;
}

/*--- Build list of S‑primes up to a given bound ------------------------*/
static void build_sp_list(uint64_t bound)
{
    sp_cnt = 0;  /* reset list */

    /* 1. primes ≡ 1 (mod 4) */
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t p = prime_list[i];
        if (p % 4 == 1) {
            ensure_sp_cap(sp_cnt + 1);
            sp_list[sp_cnt++] = (seq_t)p;
        }
    }

    /* 2. collect all primes ≡ 3 (mod 4) */
    uint32_t *p3 = NULL;
    uint64_t p3_cap = 0, p3_cnt = 0;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t p = prime_list[i];
        if (p % 4 == 3) {
            if (p3_cnt >= p3_cap) {
                p3_cap = p3_cap ? 2 * p3_cap : 64;
                uint32_t *tmp = (uint32_t *)realloc(p3, p3_cap * sizeof(uint32_t));
                p3 = tmp;
            }
            p3[p3_cnt++] = p;
        }
    }

    /* 3. products of two primes ≡ 3 (mod 4) */
    for (uint64_t i = 0; i < p3_cnt; ++i) {
        uint32_t qi = p3[i];
        if ((uint64_t)qi * qi > bound) break;
        for (uint64_t j = i; j < p3_cnt; ++j) {
            uint32_t qj = p3[j];
            unsigned __int128 prod128 = (unsigned __int128)qi * qj;
            if (prod128 > bound) break;
            ensure_sp_cap(sp_cnt + 1);
            sp_list[sp_cnt++] = (seq_t)prod128;
        }
    }
    free(p3);

    /* 4. sort the list (ascending) */
    qsort(sp_list, sp_cnt, sizeof(seq_t), cmp_seq);
}

/*--- Main function -----------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined for n=0 */

    /* static state to keep previous results across calls */
    static uint64_t current_bound = 0;
    static size_t   current_sp_cnt = 0;

    /* heuristic bound: for drawing the 10 000th term 12×n is ample. */
    uint64_t bound = n * 12ULL + 1000ULL;
    if (bound < 5000ULL) bound = 5000ULL;

    while (true) {
        if (bound > current_bound) {
            sieve_up_to(bound);
            build_sp_list(bound);
            current_bound   = bound;
            current_sp_cnt = sp_cnt;
        }
        if (current_sp_cnt >= (size_t)n) break;
        bound <<= 1;   /* not enough: double the bound */
    }

    return sp_list[n-1];
}
