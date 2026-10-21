/*
 * A056653  Composite numbers together with 1 but excluding 4.
 * https://oeis.org/A056653
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 200, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n): the nth element of the sequence of composite numbers
   together with 1 but excluding 4 (OEIS A056653).
   The algorithm generates composites incrementally, storing them in a
   static array, and uses a dynamic list of primes for primality testing. */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

static seq_t *seq = NULL;          /* array storing the sequence */
static size_t seq_cap = 0;         /* capacity of seq array */
static size_t seq_len = 0;         /* # terms stored (seq_len >= 1) */

static idx_t *primes = NULL;      /* list of primes found so far */
static size_t primes_cap = 0;      /* capacity of primes array */
static size_t primes_cnt = 0;      /* # primes stored */

static idx_t current_candidate = 2; /* next integer to test */
static bool initialized = false;

/* Ensure seq has room for at least min_cap elements. */
static void seq_ensure_cap(size_t min_cap)
{
    if (min_cap <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 64;
    if (new_cap < min_cap) new_cap = min_cap;
    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* Ensure primes has room for at least min_cap elements. */
static void primes_ensure_cap(size_t min_cap)
{
    if (min_cap <= primes_cap) return;
    size_t new_cap = primes_cap ? primes_cap * 2 : 64;
    if (new_cap < min_cap) new_cap = min_cap;
    primes = (idx_t *)realloc(primes, new_cap * sizeof(idx_t));
    primes_cap = new_cap;
}

/* Test whether x is composite using the primes list built so far. */
static bool is_composite(idx_t x)
{
    if (x < 2) return false;
    for (size_t i = 0; i < primes_cnt; ++i)
    {
        idx_t p = primes[i];
        if (p * p > x) break;
        if (x % p == 0) return true;
    }
    return false;
}

/* Add a prime to the primes list. */
static void add_prime(idx_t p)
{
    primes_ensure_cap(primes_cnt + 1);
    primes[primes_cnt++] = p;
}

/* Generate composite terms until we have at least n terms in seq. */
static void generate_terms(idx_t n)
{
    while (seq_len < n)
    {
        if (is_composite(current_candidate))
        {
            if (current_candidate != 4)    /* exclude 4 */
            {
                seq_ensure_cap(seq_len + 1);
                seq[seq_len++] = (seq_t)current_candidate;
            }
        }
        else
        {
            add_prime(current_candidate);
        }
        ++current_candidate;
    }
}

seq_t a(idx_t n)
{
    if (!initialized)
    {
        seq_ensure_cap(1);
        seq[0] = (seq_t)1;     /* a(1) = 1 */
        seq_len = 1;
        primes_ensure_cap(64);
        primes_cnt = 0;
        current_candidate = 2;
        initialized = true;
    }
    if (n < 1) return (seq_t)0;   /* undefined for n < 1, return 0 */
    generate_terms(n);
    return seq[n - 1];
}
