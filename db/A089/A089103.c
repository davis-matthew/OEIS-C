/*
 * A089103  a(n) = Mod[n+Prime[n],10].
 * https://oeis.org/A089103
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Mod[n+Prime[n],10]
 * Range:     u8 n <= 46, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>


/* static cache for primes – generated on demand */
static uint64_t *prime_arr = NULL;
static idx_t prime_cnt = 0;          /* number of primes currently stored */
static idx_t prime_cap = 0;          /* current capacity of prime_arr */
static uint64_t next_candidate = 2;  /* next integer to test for primality */

/* enlarge prime_arr to at least min_cap */
static void prime_grow(idx_t min_cap)
{
    idx_t cap = prime_cap ? prime_cap : 16;
    while (cap < min_cap) {
        cap <<= 1;
    }
    /* cast the result of realloc for C++ compatibility */
    prime_arr = (uint64_t *)realloc(prime_arr, cap * sizeof(uint64_t));
    prime_cap = cap;
}

/* generate primes until we have at least need primes stored */
static void prime_generate(idx_t need)
{
    if (need <= prime_cnt) {
        return;
    }
    prime_grow(need);
    for (; prime_cnt < need; ++prime_cnt) {
        /* find next prime by trial division */
        while (1) {
            uint64_t p = next_candidate;
            ++next_candidate;                      /* prepare next number */

            int is_prime = 1;
            for (idx_t i = 0; i < prime_cnt; ++i) {
                uint64_t q = prime_arr[i];
                if (q > p / q)                   /* q*q > p */
                    break;
                if (p % q == 0) {
                    is_prime = 0;
                    break;
                }
            }
            if (is_prime) {
                prime_arr[prime_cnt] = p;
                break;
            }
        }
    }
}

/* return the nth prime (1‑based) as a seq_t value */
static seq_t nth_prime(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;
    }
    prime_generate(n);
    return (seq_t)prime_arr[n - 1];
}

/* OEIS sequence A089103 */
seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;
    }
    seq_t p = nth_prime(n);          /* Prime[n] */
    seq_t v = (seq_t)n + p;          /* n + Prime[n] */
    return v % (seq_t)10;            /* Mod[*,10] */
}
