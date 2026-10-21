/*
 * A065762  a(n) = (sum of first n primes)^2 + sum of (squares of first n
 *          primes).
 * https://oeis.org/A065762
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (sum of first n primes)^2 + sum of (squares of first n
 *            primes)
 * Range:     u8 n <= 3, u16 n <= 13, u32 n <= 156, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static idx_t next_candidate = 2;          /* next integer to test for primeness */
static size_t prime_cap      = 0;         /* current capacity of prime array    */
static size_t prime_count    = 0;         /* number of primes currently stored  */
static idx_t *prime_vals     = NULL;      /* prime values, 0‑based indexing     */

/* Ensure that prime_vals can hold at least 'needed' primes. */
static void ensure_prime_capacity(size_t needed)
{
    if (prime_cap >= needed)
        return;
    size_t new_cap = prime_cap ? (prime_cap << 1) : 16;
    if (new_cap < needed)
        new_cap = needed;
    idx_t *p = (idx_t *)realloc(prime_vals, new_cap * sizeof(idx_t));
    if (!p)
        exit(EXIT_FAILURE);
    prime_vals = p;
    prime_cap  = new_cap;
}

/* Generate primes until we have at least 'n' of them. */
static void generate_primes_up_to(size_t n)
{
    while (prime_count < n) {
        idx_t p = next_candidate;
        int is_prime = 1;
        for (size_t i = 0; i < prime_count; ++i) {
            idx_t q = prime_vals[i];
            if (q * q > p)          /* q*q fits in idx_t for our limits */
                break;
            if (p % q == 0) {
                is_prime = 0;
                break;
            }
        }
        if (is_prime) {
            ensure_prime_capacity(prime_count + 1);
            prime_vals[prime_count] = p;
            ++prime_count;
        }
        ++next_candidate;
    }
}

/* Compute a(n) as defined in the OEIS entry. */
seq_t a(idx_t n)
{
    if (n == 0) /* offset is 1, undefined for 0; return 0 safely */
        return (seq_t)0;

    generate_primes_up_to((size_t)n);

    seq_t sum      = 0;
    seq_t sum_sq   = 0;
    for (size_t i = 0; i < (size_t)n; ++i) {
        seq_t p = (seq_t)prime_vals[i];
        sum += p;
        sum_sq += p * p;
    }
    return sum * sum + sum_sq;
}
