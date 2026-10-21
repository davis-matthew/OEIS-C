/*
 * A215311  Primes congruent to {1, 2, 3, 4, 5} mod 13.
 * https://oeis.org/A215311
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth prime congruent to 1,2,3,4 or 5 (mod 13). */
/* It incrementally tests integers, keeps all found primes, and when a
 * prime has residue 1–5 modulo 13 it is stored in an output list. */

#include <stdlib.h>
#include <stdbool.h>

/* Dynamic array growth helper – compatible with C and C++ */
static void grow_seq(seq_t **arr, size_t *cap, size_t new_cap)
{
    if (*cap >= new_cap) return;
    size_t c = (*cap == 0) ? 16 : *cap;
    while (c < new_cap) c <<= 1;
    *arr = (seq_t*)realloc(*arr, c * sizeof(seq_t));
    *cap = c;
}

/* Shared static storage. */
static seq_t *primes_all = NULL;
static size_t size_all = 0, cap_all = 0;
static seq_t *primes_ok  = NULL;
static size_t size_ok   = 0, cap_ok   = 0;
static seq_t last_candidate = (seq_t)1;

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    while (size_ok < (size_t)n)
    {
        ++last_candidate;                     /* next integer to test   */

        /* Trial division: test divisibility by all primes found so far. */
        bool isprime = true;
        for (size_t i = 0; i < size_all; ++i)
        {
            seq_t p = primes_all[i];
            if (p * p > last_candidate) break;
            if (last_candidate % p == 0) { isprime = false; break; }
        }

        if (isprime)
        {
            grow_seq(&primes_all, &cap_all, size_all + 1);
            primes_all[size_all++] = last_candidate;

            int r = (int)(last_candidate % 13);
            if (r >= 1 && r <= 5)
            {
                grow_seq(&primes_ok, &cap_ok, size_ok + 1);
                primes_ok[size_ok++] = last_candidate;
            }
        }
    }
    return primes_ok[size_ok - 1];
}
