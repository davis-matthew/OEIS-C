/*
 * A076889  Sum of aliquot divisors of palindromic numbers.
 * https://oeis.org/A076889
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = σ(nᵗ) – nᵗ where nᵗ is the n‑th decimal palindrome
 *            (1‑based) and σ(m) is the sum of all positive divisors of m
 *            (including m)
 * Range:     u8 n <= 33, u16 n <= 499, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is computed by generating the n‑th palindrome and
   then summing its proper divisors. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Build a simple prime sieve up to 100 000 for factorisation.
   This bound is sufficient for palindromes arising at n ≤ 10 000. */
static uint32_t *_prime_list = NULL;
static size_t _prime_count = 0;

static void init_primes(void)
{
    if (_prime_list != NULL) return;         /* already done */
    const size_t LIMIT = 100000;
    bool *is_comp = (bool*)malloc((LIMIT + 1) * sizeof(bool));
    for (size_t i = 2; i <= LIMIT; ++i) is_comp[i] = false;

    for (size_t i = 2; i * i <= LIMIT; ++i)
        if (!is_comp[i])
            for (size_t j = i * i; j <= LIMIT; j += i)
                is_comp[j] = true;

    /* Estimate number of primes roughly LIMIT / ln(LIMIT) */
    size_t est = LIMIT / 6;  /* generous upper bound */
    _prime_list = (uint32_t*)malloc(est * sizeof(uint32_t));
    size_t cnt = 0;
    for (size_t i = 2; i <= LIMIT; ++i)
        if (!is_comp[i]) _prime_list[cnt++] = (uint32_t)i;

    _prime_count = cnt;
    free(is_comp);
}

/* fast power of 10 for 0 ≤ e < 10 */
static uint64_t pow10_uint(unsigned e)
{
    static const uint64_t tbl[10] = {
        1ULL,10ULL,100ULL,1000ULL,10000ULL,
        100000ULL,1000000ULL,10000000ULL,100000000ULL,1000000000ULL
    };
    return tbl[e];
}

/* Return the n‑th decimal palindrome (1‑based). */
static uint64_t nth_palindrome(idx_t n)
{
    /* zero‑based index */
    uint64_t rem = (uint64_t)n - 1;
    uint64_t len = 1;

    while (true) {
        uint64_t count_len = 9ULL * pow10_uint((len - 1) / 2);
        if (rem < count_len) break;
        rem -= count_len;
        ++len;
    }

    uint64_t half_len = (len + 1) / 2;
    uint64_t start_half = pow10_uint((unsigned)half_len - 1);
    uint64_t first_half = start_half + rem;

    uint64_t pal = first_half;
    uint64_t rev = first_half;
    if (len & 1) rev /= 10;               /* drop middle digit for odd length */

    while (rev > 0) {
        pal = pal * 10 + (rev % 10);
        rev /= 10;
    }
    return pal;
}

/* Sum of proper divisors of n (n ≥ 1).  Returns 0 for n == 1. */
static uint64_t sum_proper_divisors(uint64_t n)
{
    if (n <= 1) return 0;

    uint64_t result = 1;
    uint64_t t = n;

    for (size_t i = 0; i < _prime_count && (uint64_t)_prime_list[i] * (uint64_t)_prime_list[i] <= t; ++i) {
        uint32_t p = _prime_list[i];
        if (t % p != 0) continue;

        uint64_t term = 1;
        uint64_t power = 1;
        do {
            power *= p;
            term += power;
            t /= p;
        } while (t % p == 0);

        result *= term;
    }

    if (t > 1)            /* remaining prime factor */
        result *= (1ULL + t);

    return result - n;     /* exclude the number itself */
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* sequence defined for n≥1 */
    init_primes();                          /* ensure primes are ready */

    uint64_t pal = nth_palindrome(n);
    uint64_t s   = sum_proper_divisors(pal);
    return (seq_t)s;                       /* cast is fine for any seq_t size */
}
