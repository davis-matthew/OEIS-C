/*
 * A096014  a(n) = (smallest prime factor of n) * (least prime that is not a
 *          factor of n), with a(1)=2.
 * https://oeis.org/A096014
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = A020639(n) * A053669(n) (smallest prime factor × least
 *            prime not dividing n; a(1) = 2)
 * Range:     u8 n <= 130, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"   /* build_primes_upto() */

static uint64_t *prime_list = NULL;      /* array of primes */
static size_t    prime_count = 0;        /* number of primes stored */
static bool      primes_initialized = false;

/* Initialise the global prime table once.  The chosen limit covers
   all n that appear in the test harness comfortably. */
static void init_prime_table(void)
{
    if (primes_initialized)
        return;
    const idx_t LIMIT = 100000;  /* enough for n up to at least 10 000 */
    build_primes_upto(LIMIT, &prime_list, &prime_count);
    primes_initialized = true;
}

/* Compute the least prime that does NOT divide n.  prime_list
   must be initialised before calling this function.  If all
   primes in the table divide n, continue searching beyond the
   pre‑computed range. */
static seq_t least_prime_not_dividing(uint64_t n)
{
    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = prime_list[i];
        if (n % p != 0)
            return (seq_t)p;
    }
    /* Rare case: all table primes divide n.  Search for the next prime. */
    uint64_t candidate = prime_list[prime_count - 1] + 1;
    while (true) {
        bool is_prime = true;
        for (size_t i = 0; i < prime_count && prime_list[i] * prime_list[i] <= candidate; ++i) {
            if (candidate % prime_list[i] == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime)
            return (seq_t)candidate;
        ++candidate;
    }
}

/* Compute the smallest prime factor of n.  If n is 1, return 2
   (used only for a(1), which is handled separately). */
static uint64_t smallest_prime_factor(uint64_t n)
{
    if (n < 2)
        return 2;  /* not used for n ≥ 2, but safe for safety */
    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > n)
            break;
        if (n % p == 0)
            return p;
    }
    /* n itself is prime */
    return n;
}

/* The required OEIS sequence function. */
seq_t a(idx_t n)
{
    /* Sequence offset is 1; a(1) is defined as 2. */
    if (n == 1)
        return (seq_t)2;

    if (!primes_initialized)
        init_prime_table();

    /* Cast n to uint64_t for helper computations. */
    uint64_t un = (uint64_t)n;

    /* Smallest prime factor of n. */
    uint64_t spf = smallest_prime_factor(un);

    /* Least prime not dividing n. */
    seq_t lnp = least_prime_not_dividing(un);

    /* Return the product, cast to seq_t. */
    return (seq_t)spf * lnp;
}
