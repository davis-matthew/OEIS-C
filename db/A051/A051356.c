/*
 * A051356  Start with 0, run through primes >=5, adding if -1 mod 6,
 *          subtracting if +1 mod 6.
 * https://oeis.org/A051356
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0)=0; for n≥1, a(n)=a(n−1)+
 *            ((prime(n+2)%6==5)?prime(n+2):−prime(n+2)) where prime(k) is the
 *            k‑th prime (prime(1)=2). This is equivalent to a(n)=∑_{p≥5,
 *            p≤prime(n+2)} sgn(p)*p with sgn(p)=+1 if p≡5 (mod 6), −1 if p≡1
 *            (mod 6), and 0 for 2,3
 * Range:     i8 n <= 14, i16 n <= 706, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/*--- prime storage -----------------------------------------------------------*/
static uint32_t *prime_list   = NULL;   /* zero–based: prime_list[0]=2, [1]=3, [2]=5,... */
static size_t   prime_cnt     = 0;      /* number of primes stored so far          */
static size_t   prime_cap     = 0;      /* malloced capacity                       */

/* ensure prime_list can hold at least new_cap elements */
static void ensure_prime_capacity(size_t new_cap)
{
    if (new_cap <= prime_cap) return;
    size_t cap = prime_cap ? prime_cap : 16;
    while (cap < new_cap) cap <<= 1;
    uint32_t *p = (uint32_t *)realloc(prime_list, cap * sizeof(uint32_t));
    if (!p) abort();
    prime_list = p;
    prime_cap  = cap;
}

/* generate primes until prime_list[need_index] is valid (0‑based) */
static void generate_primes_until(size_t need_index)
{
    if (prime_cnt == 0) {
        /* initialise with first two primes */
        ensure_prime_capacity(4);
        prime_list[0] = 2;
        prime_list[1] = 3;
        prime_cnt    = 2;
    }
    while (prime_cnt <= need_index) {
        uint32_t candidate = prime_list[prime_cnt-1] + 2;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint32_t p = prime_list[i];
            if ((uint64_t)p * p > (uint64_t)candidate) break;
            if (candidate % p == 0) { candidate += 2; i = -1; }
        }
        ensure_prime_capacity(prime_cnt + 1);
        prime_list[prime_cnt++] = candidate;
    }
}

/*--- cumulative sum storage -------------------------------------------------*/
static seq_t *cum_sum = NULL;          /* cum_sum[0]=0, cum_sum[n]=a(n)           */
static size_t cum_used = 0;            /* highest index computed (≥0)             */
static size_t cum_cap  = 0;            /* malloced capacity                       */

/* ensure cum_sum can hold indices up to idx (inclusive) */
static void ensure_cum_size(size_t idx)
{
    if (idx < cum_cap) return;
    size_t cap = cum_cap ? cum_cap : 8;
    while (cap <= idx) cap <<= 1;
    seq_t *p = (seq_t *)realloc(cum_sum, cap * sizeof(seq_t));
    if (!p) abort();
    cum_sum = p;
    cum_cap = cap;
}

/*--- sequence function -----------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;  /* a(0)=0 */

    size_t u = (size_t)n;                 /* use size_t for indexing */
    ensure_cum_size(u);

    if (cum_used == 0) {                  /* initialise a(0) if needed */
        cum_sum[0] = (seq_t)0;
        cum_used   = 0;
    }

    for (size_t idx = cum_used + 1; idx <= u; ++idx) {
        /* idx corresponds to a(idx).  The prime used is prime idx+1
           (zero‑based: p2=3->idx=0 gives prime index 1 which is 3, but
           we skip 2 and 3 by starting at idx=1). */
        size_t prime_index = idx + 1;       /* zero‑based index in prime_list */
        generate_primes_until(prime_index);
        seq_t prime = (seq_t)prime_list[prime_index];
        seq_t signed_prime = (prime % 6 == 5) ? prime : -(prime);
        cum_sum[idx] = cum_sum[idx - 1] + signed_prime;
    }
    cum_used = u;
    return cum_sum[u];
}
