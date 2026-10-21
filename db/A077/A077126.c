/*
 * A077126  Sum of even-indexed primes.
 * https://oeis.org/A077126
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of the first n even-indexed primes
 * Range:     u8 n <= 8, u16 n <= 107, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>
#include <stdint.h>

/* (a(1)=2nd prime, a(2)=2nd+4th prime, …)                 */

static uint32_t *primes = NULL;          /* all primes found so far          */
static size_t primes_cnt = 0;           /* number of primes stored          */
static size_t primes_cap = 0;           /* allocated capacity of primes     */
static uint32_t next_candidate = 2;      /* next number to test for primality */

static seq_t *evenSum = NULL;           /* evenSum[i] = sum of first i even-indexed primes (evenSum[0]=0) */
static size_t even_cnt = 0;             /* number of even-indexed primes stored      */
static size_t even_cap = 0;             /* allocated capacity of evenSum            */

/* Ensure that the static arrays are initialized once. */
static void init_arrays(void)
{
    if (primes == NULL) {
        primes_cap = 16;
        primes = (uint32_t *)malloc(primes_cap * sizeof(uint32_t));
        even_cap = 1;
        evenSum = (seq_t *)malloc(even_cap * sizeof(seq_t));
        evenSum[0] = (seq_t)0;
    }
}

/* Double the capacity of the prime array. */
static void grow_primes(void)
{
    size_t new_cap = primes_cap ? primes_cap * 2 : 16;
    uint32_t *tmp = (uint32_t *)realloc(primes, new_cap * sizeof(uint32_t));
    if (!tmp) exit(1);   /* terminate on allocation failure */
    primes = tmp;
    primes_cap = new_cap;
}

/* Double the capacity of the evenSum array. */
static void grow_even(void)
{
    size_t new_cap = even_cap ? even_cap * 2 : 2;
    seq_t *tmp = (seq_t *)realloc(evenSum, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);
    evenSum = tmp;
    even_cap = new_cap;
}

/* Store a newly found prime and update evenSum if necessary. */
static void add_prime(uint32_t p)
{
    if (primes_cnt == primes_cap) grow_primes();
    primes[primes_cnt++] = p;

    size_t idx = primes_cnt;    /* primes are 1‑based indexed */
    if (idx % 2 == 0) {         /* even-indexed prime */
        if (even_cnt + 1 == even_cap) grow_even();
        evenSum[even_cnt + 1] = evenSum[even_cnt] + (seq_t)p;
        even_cnt++;
    }
}

/* Ensure that at least “need” primes (indices 1 … need) are stored. */
static void ensure_primes(size_t need)
{
    init_arrays();
    while (primes_cnt < need) {
        uint32_t num = next_candidate;
        ++next_candidate;

        /* trial division by known primes up to sqrt(num) */
        uint32_t limit = (uint32_t)floor(sqrt((double)num));
        int isPrime = 1;
        for (size_t i = 0; i < primes_cnt && primes[i] <= limit; ++i) {
            if (num % primes[i] == 0) {
                isPrime = 0;
                break;
            }
        }
        if (isPrime) add_prime(num);
    }
}

/* Public function required by the build system. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* safety for incomplete offset */

    /* We need primes up to index 2*n to have n even‑indexed primes. */
    ensure_primes((size_t)(2 * n));

    return evenSum[(size_t)n];
}
