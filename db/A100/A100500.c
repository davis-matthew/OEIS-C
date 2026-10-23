/*
 * A100500  a(n) = prime(3n-2) + prime(3n-1) + prime(3n).
 * https://oeis.org/A100500
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(3n-2) + prime(3n-1) + prime(3n)
 * Range:     u8 n <= 8, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Dynamically store the list of primes found so far. */
static idx_t    *prime_list   = NULL;
static size_t    prime_count  = 0;
static size_t    prime_capacity = 0;

/* Grow the prime list to accommodate at least 'min_cap' primes. */
static void grow_primes_capacity(size_t min_cap) {
    if (min_cap <= prime_capacity) return;
    size_t new_cap = prime_capacity ? prime_capacity * 2 : 16;
    while (new_cap < min_cap) new_cap *= 2;
    /* realloc returns void* in C++; C-style cast works in both C and C++ */
    idx_t *tmp = (idx_t *)realloc(prime_list, new_cap * sizeof(idx_t));
    if (!tmp) abort();              /* allocation failed */
    prime_list   = tmp;
    prime_capacity = new_cap;
}

/* Generate the next prime by checking divisibility against known primes. */
static void generate_next_prime(void) {
    if (prime_count == 0) {
        grow_primes_capacity(1);
        prime_list[0] = 2;
        prime_count   = 1;
        return;
    }
    idx_t candidate = prime_list[prime_count - 1] + 1;
    if ((candidate & 1) == 0) candidate++; /* start with odd number */

    for (;;) {
        int is_prime = 1;
        for (size_t i = 0; i < prime_count; ++i) {
            idx_t p = prime_list[i];
            if (p * p > candidate) break;
            if (candidate % p == 0) { is_prime = 0; break; }
        }
        if (is_prime) {
            grow_primes_capacity(prime_count + 1);
            prime_list[prime_count++] = candidate;
            return;
        }
        candidate += 2;          /* skip even numbers */
    }
}

/* Ensure that at least 'need' primes are available. */
static void ensure_primes_up_to(idx_t need) {
    if (prime_count >= need) return;
    for (idx_t i = prime_count; i < need; ++i)
        generate_next_prime();
}

/* Sequence: a(n) = prime(3n-2) + prime(3n-1) + prime(3n) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* behaviour below offset not specified */
    idx_t k = 3 * n;                /* need primes up to index 3n */
    ensure_primes_up_to(k);
    /* indices are 0‑based in prime_list */
    seq_t sum = 0;
    sum += (seq_t)prime_list[k - 3];
    sum += (seq_t)prime_list[k - 2];
    sum += (seq_t)prime_list[k - 1];
    return sum;
}
