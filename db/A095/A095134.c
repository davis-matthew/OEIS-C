/*
 * A095134  Sum of the product of the first ceiling(n/2) odd-indexed primes
 *          and the product of the first floor(n/2) even-indexed primes; a(1)
 *          = 2.
 * https://oeis.org/A095134
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (∏_{k=1}^{ceil(n/2)} p_{2k-1}) + (∏_{k=1}^{floor(n/2)}
 *            p_{2k}) (when floor(n/2)=0 the second product is 0; when
 *            ceil(n/2)=0 the first product is 0 – this only occurs for n=0,
 *            which is treated as 0)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 15, u64 n <= 25, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t *prime_list   = NULL;   /* stored primes, increasing order */
static size_t   prime_capacity = 0;     /* allocated size of prime_list */
static idx_t    prime_count    = 0;     /* number of primes stored */
static uint64_t prime_candidate = 2;    /* next integer to test for primality */

/* Ensure prime_list contains at least need primes. */
static void ensure_primes(idx_t need)
{
    /* allocate or grow storage if needed */
    if (prime_capacity < (size_t)need) {
        size_t new_cap = prime_capacity > 0 ? prime_capacity * 2 : 64;
        while (new_cap < (size_t)need) new_cap *= 2;
        prime_list = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
        if (!prime_list) abort();
        prime_capacity = new_cap;
    }

    /* Generate primes until we have enough */
    while (prime_count < need) {
        uint64_t p = prime_candidate;
        int is_prime = 1;
        for (size_t j = 0; j < prime_count; ++j) {
            uint64_t q = prime_list[j];
            if (q * q > p) break;
            if (p % q == 0) { is_prime = 0; break; }
        }
        if (is_prime) {
            prime_list[prime_count++] = p;
        }

        /* increment candidate: skip even numbers after 2 */
        if (p == 2) prime_candidate = 3;
        else prime_candidate += 2;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for offset 1, harmless otherwise */

    /* Ensure we have the first n primes */
    ensure_primes(n);

    seq_t odd_prod  = 1;
    seq_t even_prod = 1;
    idx_t even_cnt  = 0;

    for (idx_t i = 1; i <= n; ++i) {
        seq_t p = (seq_t)prime_list[i-1];   /* primes are 1‑based here */
        if (i & 1) {                       /* odd index */
            odd_prod *= p;
        } else {                           /* even index */
            even_prod *= p;
            ++even_cnt;
        }
    }

    /* Empty product for even indices is 0 when n<2 */
    if (even_cnt == 0) even_prod = 0;

    return odd_prod + even_prod;
}
