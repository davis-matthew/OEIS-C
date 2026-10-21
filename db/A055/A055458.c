/*
 * A055458  a(n) = smallest composite solution x to the equation phi(x+2n) =
 *          phi(x)+2n.
 * https://oeis.org/A055458
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 22, u32 n <= 22, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = smallest composite x such that φ(x+2n) = φ(x)+2n */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* --- simple prime sieve to help factorisation and primality testing --- */
static unsigned *prime_list = NULL;   /* primes in ascending order */
static size_t prime_cnt   = 0;        /* number of primes stored */
static bool   primes_init = false;   /* whether prime_list is ready */

/* initialise a prime list up to a fixed limit (sufficient for n ≤ 10000) */
static void init_primes(void)
{
    if (primes_init) return;

    const size_t LIMIT = 1000000;                      /* 1 M → 78498 primes */
    bool *is_comp = (bool *)calloc(LIMIT + 1, 1);      /* 0 = prime, 1 = composite */

    /* we anticipate at most 80000 primes, grow as needed */
    size_t arr_cap = 80000;
    prime_list = (unsigned *)malloc(arr_cap * sizeof(unsigned));

    for (size_t i = 2; i <= LIMIT; ++i) {
        if (!is_comp[i]) {
            if (prime_cnt == arr_cap) {                     /* grow the array */
                arr_cap *= 2;
                prime_list = (unsigned *)realloc(prime_list,
                                                arr_cap * sizeof(unsigned));
            }
            prime_list[prime_cnt++] = (unsigned)i;

            if (i * i <= LIMIT) {
                for (size_t j = i * i; j <= LIMIT; j += i)
                    is_comp[j] = true;
            }
        }
    }
    free(is_comp);
    primes_init = true;
}

/* totient for 64‑bit unsigned integers using the pre‑computed primes */
static inline unsigned long long phi_u64(unsigned long long n)
{
    unsigned long long result = n;
    unsigned long long tmp     = n;

    for (size_t i = 0; i < prime_cnt &&
                       (unsigned long long)prime_list[i] *
                       (unsigned long long)prime_list[i] <= tmp; ++i) {
        unsigned p = prime_list[i];
        if (tmp % p == 0) {
            result -= result / p;
            while (tmp % p == 0) tmp /= p;
        }
    }
    if (tmp > 1)            /* tmp itself is prime */
        result -= result / tmp;

    return result;
}

/* primality test for 64‑bit unsigned integers using the same prime list */
static inline bool is_prime_u64(unsigned long long n)
{
    if (n < 2) return false;
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;
    if (n % 3 == 0) return n == 3;

    for (size_t i = 0; i < prime_cnt &&
                       (unsigned long long)prime_list[i] *
                       (unsigned long long)prime_list[i] <= n; ++i) {
        unsigned p = prime_list[i];
        if (n % p == 0) return false;
    }
    return true;
}

/* composite detection – composite iff > 1 and not prime */
static inline bool is_composite_u64(unsigned long long n)
{
    return n > 1 && !is_prime_u64(n);
}

/* ------------------------------------------------------------ */
/* main sequence routine                                         */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* ensure primes are ready */
    init_primes();

    /* target difference 2*n, computed in 64‑bit arithmetic */
    const unsigned long long target = 2ULL * (unsigned long long)n;

    for (unsigned long long x = 4ULL; ; ++x) {            /* smallest composite is 4 */
        if (!is_composite_u64(x)) continue;               /* skip primes */

        unsigned long long phi_x = phi_u64(x);
        unsigned long long y     = x + 2ULL * (unsigned long long)n;
        unsigned long long phi_y = phi_u64(y);

        if (phi_y - phi_x == target)
            return (seq_t)x;                                 /* cast to seq_t */
    }
}
