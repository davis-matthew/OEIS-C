/*
 * A120637  Primes such that their triple is 2 away from a prime number.
 * https://oeis.org/A120637
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A120637:
   primes p such that 3p ± 2 is prime. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* internal state */
static struct {
    idx_t limit;          /* current sieve limit */
    idx_t *primes;        /* array of primes up to limit */
    size_t prime_count;   /* number of primes stored */
    idx_t prime_index;    /* next prime to process */
    seq_t *seq;           /* sequence values computed so far */
    size_t seq_count;     /* number of sequence elements stored */
    size_t seq_cap;       /* capacity of seq array */
} state = {0, NULL, 0, 0, NULL, 0, 0};

/* simple sieve that generates all primes up to `limit` */
static void generate_primes_to(idx_t limit)
{
    bool *is_prime = (bool *)malloc((size_t)(limit + 1) * sizeof(bool));
    if (!is_prime) exit(1);
    for (idx_t i = 0; i <= limit; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (idx_t i = 2; i * i <= limit; ++i)
        if (is_prime[i])
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[j] = false;

    /* count primes */
    size_t count = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i]) ++count;

    idx_t *new_primes = (idx_t *)malloc(count * sizeof(idx_t));
    if (!new_primes) exit(1);
    size_t k = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i]) new_primes[k++] = i;

    free(state.primes);
    free(is_prime);
    state.primes      = new_primes;
    state.prime_count = count;
    state.limit       = limit;
}

/* trial division using stored primes (good for 64‑bit numbers) */
static bool is_prime_u64(uint64_t x, idx_t *primes, size_t prime_count)
{
    if (x < 2) return false;
    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = primes[i];
        if (p * p > x) break;
        if (x % p == 0) return false;
    }
    return true;
}

/* ensure enough capacity in sequence array and append a value */
static void append_seq(seq_t val)
{
    if (state.seq_count == state.seq_cap) {
        size_t new_cap = state.seq_cap ? state.seq_cap * 2 : 16;
        seq_t *new_seq = (seq_t *)realloc(state.seq, new_cap * sizeof(seq_t));
        if (!new_seq) exit(1);
        state.seq = new_seq;
        state.seq_cap = new_cap;
    }
    state.seq[state.seq_count++] = val;
}

/* main function requested by the build */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;              /* out of range, not expected */

    /* generate sequence until we have at least n elements */
    while (state.seq_count < n) {
        /* if we have exhausted the current sieve, extend it */
        if (state.prime_index >= state.prime_count) {
            idx_t new_limit = (state.limit == 0) ? 10 : state.limit * 2;
            generate_primes_to(new_limit);
        }

        idx_t p = state.primes[state.prime_index++];
        /* test 3*p-2 and 3*p+2 for primality */
        uint64_t x1 = 3ULL * (uint64_t)p - 2;
        uint64_t x2 = 3ULL * (uint64_t)p + 2;
        bool ok = is_prime_u64(x1, state.primes, state.prime_count) ||
                  is_prime_u64(x2, state.primes, state.prime_count);
        if (ok) append_seq((seq_t)p);
    }

    return state.seq[n - 1];
}
