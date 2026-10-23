/*
 * A139880  Primes of the form 13x^2+2xy+13y^2.
 * https://oeis.org/A139880
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   primes of the form 13x²+2xy+13y², equivalently primes ≡ 13, 61
 *            or 157 (mod 168)
 * Range:     u8 n <= 5, u16 n <= 410, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Helper: check the congruence condition */
static inline bool is_representable(uint64_t p)
{
    uint64_t m = p % 168ULL;
    return (m == 13ULL) || (m == 61ULL) || (m == 157ULL);
}

/* Prime generator state -----------------------------------------------*/
static uint64_t *prime_list     = NULL;   /* primes found so far */
static size_t    prime_cnt      = 0;      /* number of stored primes   */
static size_t    prime_cap      = 0;      /* allocated capacity        */
static uint64_t  next_candidate = 2;      /* next integer to test      */

/* Grow the prime list when needed */
static void grow_prime_list(void)
{
    size_t new_cap = prime_cap ? prime_cap * 2 : 16;
    uint64_t *tmp = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
    if (!tmp) abort();                     /* out of memory */
    prime_list = tmp;
    prime_cap  = new_cap;
}

/* Generate the next prime number and store it in *prime_out */
static void generate_next_prime(uint64_t *prime_out)
{
    while (true) {
        bool is_prime = true;
        for (size_t k = 0; k < prime_cnt; ++k) {
            uint64_t p = prime_list[k];
            if (p * p > next_candidate) break;
            if (next_candidate % p == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            *prime_out = next_candidate;
            if (prime_cnt >= prime_cap) grow_prime_list();
            prime_list[prime_cnt++] = next_candidate;
            ++next_candidate;
            return;
        }
        ++next_candidate;
    }
}

/* Special primes (those of the given form) state ----------------------*/
static uint64_t *special_list = NULL;     /* primes satisfying the form */
static size_t    special_cnt  = 0;        /* number of stored special primes */
static size_t    special_cap  = 0;        /* allocated capacity of special_list */

/* Grow the special list when needed */
static void grow_special_list(void)
{
    size_t new_cap = special_cap ? special_cap * 2 : 16;
    uint64_t *tmp = (uint64_t *)realloc(special_list, new_cap * sizeof(uint64_t));
    if (!tmp) abort();                     /* out of memory */
    special_list = tmp;
    special_cap  = new_cap;
}

/* ---------------------------------------------------------------------*/
/*  External linkage function requested by the build system             */
/* ---------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n < 1) return 0;                  /* undefined for n = 0 */

    /* Generate primes until we have found n special primes. */
    while (special_cnt < (size_t)n) {
        uint64_t p;
        generate_next_prime(&p);
        if (is_representable(p)) {
            if (special_cnt >= special_cap) grow_special_list();
            special_list[special_cnt++] = p;
        }
    }

    /* Convert to seq_t – this works for both plain integer types
       and for classes that provide a constructor from uint64_t. */
    return (seq_t)special_list[n - 1];
}
