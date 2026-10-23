/*
 * A117461  Indices associated with primes in A117460. Both primes and their
 *          indices, after calculation of their respective digit sums, bear
 *          the relationship that both are prime and that sod(i) < sod(p) and
 *          sod(p) is the next prime after to sod(i), where sod is the sum of
 *          digits function.
 * https://oeis.org/A117461
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   A117461 – indices i for which the i‑th prime p has digit sums
 *            sod(i) and sod(p) that are prime, sod(i) < sod(p), and sod(p) is
 *            the next prime after sod(i). (The case sod(i)=1 occurs only for
 *            i=1 and is accepted.)
 * Range:     u8 n <= 5, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   is_prime_small, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_small.h"
#include "../utils/next_prime.h"

/* grow a dynamic array of seq_t */
static void grow_seq_array(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap >= need) return;
    size_t newcap = (*cap) ? (*cap << 1) : 64;
    while (newcap < need) newcap <<= 1;
    *arr = (seq_t *)realloc(*arr, newcap * sizeof(seq_t));
    *cap = newcap;
}
/* grow a dynamic array of uint64_t */
static void grow_uint64_array(uint64_t **arr, size_t *cap, size_t need)
{
    if (*cap >= need) return;
    size_t newcap = (*cap) ? (*cap << 1) : 64;
    while (newcap < need) newcap <<= 1;
    *arr = (uint64_t *)realloc(*arr, newcap * sizeof(uint64_t));
    *cap = newcap;
}

/* sum of decimal digits of an unsigned integer of any size */
static seq_t digit_sum_idx(idx_t x)
{
    seq_t sum = 0;
    while (x != 0) {
        sum += (seq_t)(x % 10);
        x /= 10;
    }
    return sum;
}
static seq_t digit_sum_u64(uint64_t x)
{
    seq_t sum = 0;
    while (x != 0) {
        sum += (seq_t)(x % 10);
        x /= 10;
    }
    return sum;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* static state */
    static seq_t *terms = NULL;
    static size_t terms_cap = 0;
    static size_t terms_len = 0;

    static uint64_t *primes = NULL;
    static size_t primes_cap = 0;
    static size_t primes_len = 0;
    static uint64_t prime_candidate = 2;

    static idx_t next_i = 1;          /* next i to examine */

    /* initialise static storage on first call */
    if (!terms) {
        terms_cap = 64;
        terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
    }
    if (!primes) {
        primes_cap = 64;
        primes = (uint64_t *)malloc(primes_cap * sizeof(uint64_t));
    }

    /* generate terms until we have at least n+1 of them */
    while (terms_len <= n) {
        idx_t i = next_i;        /* candidate index */
        bool accept = false;

        /* ensure we have the i‑th prime */
        while (primes_len < (size_t)i) {
            if (is_prime_small((seq_t)prime_candidate)) {
                grow_uint64_array(&primes, &primes_cap, primes_len + 1);
                primes[primes_len++] = prime_candidate;
            }
            /* advance candidate: 2 → 3, then odds only */
            if (prime_candidate == 2) prime_candidate = 3;
            else prime_candidate += 2;
        }
        uint64_t p = primes[(size_t)(i - 1)];    /* the i‑th prime */

        /* compute digit sums */
        seq_t sod_i = digit_sum_idx(i);
        seq_t sod_p = digit_sum_u64(p);

        /* digit sum of i must be prime, except i==1 */
        bool sod_i_prime = (i == 1) ? true : is_prime_small(sod_i);
        bool sod_p_prime = is_prime_small(sod_p);

        if (sod_i_prime && sod_p_prime && sod_i < sod_p) {
            /* check that sod_p is the next prime after sod_i */
            uint64_t next_after = next_prime((uint64_t)sod_i);
            if (next_after == (uint64_t)sod_p) {
                accept = true;
            }
        }

        if (accept) {
            grow_seq_array(&terms, &terms_cap, terms_len + 1);
            terms[terms_len++] = (seq_t)i;
        }
        ++next_i;   /* move to the next candidate index */
    }
    return terms[n];
}
