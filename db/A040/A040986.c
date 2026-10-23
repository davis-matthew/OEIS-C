/*
 * A040986  Primes p such that x^5 = 21 has a solution mod p.
 * https://oeis.org/A040986
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 43, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes p such that x^5 ≡ 21 (mod p) has a solution */

/* Computes the n-th term of OEIS sequence A040986.
   The sequence is the set of primes p for which the congruence
   x^5 ≡ 21 (mod p) is solvable.  For primes p∤21 this holds
   iff 21^((p−1)/g) ≡ 1 (mod p) where g = gcd(5, p−1).
   Since g is 1 or 5, all primes with p ≠ 1 (mod 5) satisfy
   the condition, and for p ≡ 1 (mod 5) the extra modular
   exponentiation test is performed. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/mod_pow.h"

static seq_t *primes = NULL;       /* cached primes in the sequence   */
static size_t primes_len = 0;      /* number of primes cached         */
static size_t primes_cap = 0;      /* allocated capacity              */
static idx_t next_candidate = 2;   /* next integer to test for prime  */

static void ensure_capacity(size_t needed)
{
    if (needed <= primes_cap) return;
    size_t new_cap = primes_cap ? primes_cap * 2 : 8;
    while (new_cap < needed) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(primes, new_cap * sizeof(seq_t));
    if (!tmp) abort();              /* fallback on allocation failure */
    primes = tmp;
    primes_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n < 1, but guard */
    while (primes_len < n)
    {
        /* Skip even numbers after 2, but still test 2 itself. */
        if (next_candidate == 2)
        {
            /* 2 is always included because 2%5 != 1. */
            ensure_capacity(primes_len + 1);
            primes[primes_len++] = next_candidate;
            ++next_candidate;
            continue;
        }
        /* Increment candidate to next odd number, so we only test odds. */
        if (next_candidate > 2 && (next_candidate & 1ULL) == 0) next_candidate++;

        /* Test primality. */
        if (!is_prime(next_candidate))
        {
            ++next_candidate;
            continue;
        }

        bool include = false;
        if (next_candidate == 3 || next_candidate == 7)
        {
            /* p divides 21 → trivial solution x ≡ 0 (mod p). */
            include = true;
        }
        else if ((next_candidate % 5) != 1)
        {
            /* gcd(5, p-1) = 1 → condition always satisfied. */
            include = true;
        }
        else
        {
            /* We need to test 21^((p-1)/5) ≡ 1 (mod p). */
            seq_t val = mod_pow((seq_t)21,
                                (next_candidate - 1) / 5,
                                (seq_t)next_candidate);
            if (val == (seq_t)1) include = true;
        }

        if (include)
        {
            ensure_capacity(primes_len + 1);
            primes[primes_len++] = next_candidate;
        }
        ++next_candidate;
    }
    return primes[n - 1];
}
