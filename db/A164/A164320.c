/*
 * A164320  Primes p such that sums of divisors of the two adjacent integers
 *          are each > 2*p.
 * https://oeis.org/A164320
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 727, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A164320
   Formula: a(n) = the n‑th prime p such that sigma(p-1) > 2p and sigma(p+1) > 2p
   (intersection of A164318 and A164319). */
/* The function uses an incremental prime sieve and divisor‑sum evaluation.
   It stores all qualifying primes in a static array so that multiple calls
   can be handled in any order. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>

/* --- Integer square root ------------------------------------------------ */
static inline uint64_t isqrt_u64(uint64_t n) {
    uint64_t r = (uint64_t)sqrt((long double)n);
    while ((r + 1) * (r + 1) <= n) ++r;
    while (r * r > n) --r;
    return r;
}

/* --- Prime list management ---------------------------------------------- */
static uint64_t *prime_list   = NULL; /* stored primes, starting with 2      */
static size_t   prime_cnt     = 0;    /* number of primes                    */
static size_t   prime_cap     = 0;    /* allocated capacity                  */

static void init_primes(void) {
    if (prime_cnt > 0) return;          /* already initialized                */
    prime_cap = 16;
    prime_list = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
    if (!prime_list) exit(1);
    prime_list[0] = 2;
    prime_cnt = 1;
}

static void add_prime(uint64_t p) {
    if (prime_cnt == prime_cap) {
        size_t new_cap = prime_cap * 2;
        uint64_t *tmp = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
        if (!tmp) exit(1);
        prime_list = tmp;
        prime_cap = new_cap;
    }
    prime_list[prime_cnt++] = p;
}

/* --- Divisor‑sum sigma for uint64_t -------------------------------------- */
static seq_t sigma_uint64(uint64_t n) {
    seq_t res = 1;
    uint64_t m   = n;
    for (size_t i = 0; i < prime_cnt && prime_list[i] <= isqrt_u64(m); ++i) {
        uint64_t p = prime_list[i];
        if (m % p == 0) {
            uint64_t exp = 0;
            while (m % p == 0) {
                m  /= p;
                exp++;
            }
            /* Compute term = (p^(exp+1)-1)/(p-1)  */
            seq_t pow = 1;
            for (uint64_t k = 0; k <= exp; ++k) pow *= (seq_t)p;
            seq_t term = (pow - 1) / ((seq_t)p - 1);
            res *= term;
        }
    }
    if (m > 1) res *= (seq_t)(1 + m);   /* remaining prime factor   */
    return res;
}

/* --- Qualifying primes buffer ------------------------------------------- */
static seq_t *qual_prime_list = NULL; /* primes satisfying the condition   */
static size_t qual_cnt        = 0;
static size_t qual_cap        = 0;

/* --- Candidate arithmetic ---------------------------------------------- */
static uint64_t candidate = 3;        /* next odd number to test           */

/* --- Main sequence function --------------------------------------------- */
seq_t a(idx_t n) {
    if (n == 0) return 0;             /* safety, real sequence defined for n>=1 */
    init_primes();                    /* ensure prime list contains 2 */

    /* Ensure we have at least n qualifying primes */
    while (qual_cnt < n) {
        /* ----- Test if candidate is prime --------------------------- */
        bool is_prime = true;
        uint64_t root = isqrt_u64(candidate);
        for (size_t i = 0; i < prime_cnt && prime_list[i] <= root; ++i) {
            if (candidate % prime_list[i] == 0) {
                is_prime = false;
                break;
            }
        }

        if (is_prime) {
            /* Store new prime */
            add_prime(candidate);

            /* Check the defining condition for p = candidate */
            seq_t sigma1 = sigma_uint64(candidate - 1);
            seq_t sigma2 = sigma_uint64(candidate + 1);
            seq_t bound  = (seq_t)2 * (seq_t)candidate;

            if (sigma1 > bound && sigma2 > bound) {
                /* Store in qualifying list */
                if (qual_cnt == qual_cap) {
                    size_t new_cap = qual_cap ? qual_cap * 2 : 16;
                    seq_t *tmp = (seq_t *)realloc(qual_prime_list, new_cap * sizeof(seq_t));
                    if (!tmp) exit(1);
                    qual_prime_list = tmp;
                    qual_cap = new_cap;
                }
                qual_prime_list[qual_cnt++] = (seq_t)candidate;
            }
        }

        candidate += 2;   /* next odd candidate */
    }

    return qual_prime_list[n - 1];
}
