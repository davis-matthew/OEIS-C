/*
 * A161892  Numerators of S(n) = Sum_{j=2..n} (pi((j+1)^2) -
 *          pi(j^2))/(pi((j+1)^2)*pi(j^2)) where pi(k) = A000720(k).
 * https://oeis.org/A161892
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, frac, less
 * Formula:   The sequence defined in OEIS A161892 a(n) = numerator of S(n)
 *            where S(n) = ∑_{j=2}^{n} (pi((j+1)^2)-pi(j^2)) /
 *            (pi((j+1)^2)*pi(j^2)) pi(k) = number of primes ≤ k
 * Range:     u8 none, u16 none, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   gcd, is_bitset_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/gcd.h"
#include "../utils/is_bitset_prime.h"

#define MAX_N 10000   /* maximum n that may be requested (offset 2) */

static seq_t *numerators = NULL;   /* numerators for indices >=2 */
static size_t computed_max = 0;    /* highest index computed */

/* Build a bitset sieve up to LIMIT and compute the numerators array. */
static void init_sequence(void)
{
    if (numerators != NULL)           /* already initialised */
        return;

    /* 1. Allocate the bitset for all numbers up to (MAX_N+1)^2.              */
    uint64_t limit = (uint64_t)(MAX_N + 1) * (uint64_t)(MAX_N + 1);
    size_t bits_len = (limit >> 3) + 1;
    unsigned char *bits = (unsigned char *)malloc(bits_len);
    if (bits == NULL)
        return;
    memset(bits, 0xFF, bits_len);    /* all numbers are initially prime */

    /* 0 and 1 are not prime. */
    bits[0] &= ~((unsigned char)1 << 0);
    bits[0] &= ~((unsigned char)1 << 1);

    /* Mark even numbers >2 as composite. */
    for (uint64_t p = 4; p <= limit; p += 2)
        bits[p >> 3] &= ~((unsigned char)1 << (p & 7));

    /* Sieve odds starting from 3. */
    for (uint64_t p = 3; p * p <= limit; p += 2)
    {
        if (!is_bitset_prime(bits, p))
            continue;
        for (uint64_t m = p * p; m <= limit; m += p << 1) /* j += 2p */
            bits[m >> 3] &= ~((unsigned char)1 << (m & 7));
    }

    /* 2. Allocate array for numerators. */
    numerators = (seq_t *)calloc((size_t)MAX_N + 2, sizeof(seq_t));
    if (numerators == NULL) {
        free(bits);
        return;
    }

    /* 3. Compute pi(j^2) for j = 2 .. MAX_N+1 and the cumulative numerators. */
    seq_t num = 0;   /* current numerator of S(j) */
    seq_t den = 1;   /* current denominator (irreducible) */
    uint32_t prime_cnt = 0;
    uint64_t cur_index = 2;   /* current number being inspected to count primes */

    for (uint64_t j = 2; j <= (uint64_t)MAX_N + 1; ++j)
    {
        uint64_t sq = j * j;
        while (cur_index <= sq)
        {
            if (is_bitset_prime(bits, cur_index))
                ++prime_cnt;
            ++cur_index;
        }
        seq_t pi_j      = (seq_t)prime_cnt;

        /* next square value */
        uint64_t sq_next = (j + 1) * (j + 1);
        while (cur_index <= sq_next)
        {
            if (is_bitset_prime(bits, cur_index))
                ++prime_cnt;
            ++cur_index;
        }
        seq_t pi_next = (seq_t)prime_cnt;

        seq_t term_num = pi_next - pi_j;
        seq_t term_den = pi_next * pi_j;

        seq_t new_num = num * term_den + term_num * den;
        seq_t new_den = den * term_den;
        seq_t g = gcd(new_num, new_den);
        num = new_num / g;
        den = new_den / g;

        numerators[(size_t)j] = num;   /* store numerator for a(j) */
    }

    free(bits);
    computed_max = MAX_N + 1;
}

/* Public function required by the build system. */
seq_t a(idx_t n)
{
    if (n < 2)
        return 0;                 /* sequence is undefined below offset */
    init_sequence();               /* ensure data is ready */
    if ((size_t)n <= computed_max)
        return numerators[(size_t)n];
    return 0;                     /* should not happen for n ≤ MAX_N+1 */
}
