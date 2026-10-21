/*
 * A070749  Nearest integer to sin(prime(n)), prime=A000040.
 * https://oeis.org/A070749
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 109, i16 n <= 109, i32 n <= 109, i64 n <= 109,
 *            i128 n <= 109, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A070749 – nearest integer to sin(p_n),  p_n = n‑th prime  */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/*  -----  Prime list cache  -----  */
static uint32_t *prime_list  = NULL;  /* array of primes */
static size_t   prime_count = 0;      /* number of primes stored */

/*  build_primes – simple Eratosthenes sieve up to ‘limit’.  */
static void build_primes(uint32_t limit)
{
    bool *is_comp = (bool *)calloc(limit + 1, sizeof(bool));
    if (!is_comp) return;                         /* out of memory – give up */

    for (uint32_t p = 2; (uint64_t)p * p <= limit; ++p)
        if (!is_comp[p])
            for (uint64_t q = (uint64_t)p * p; q <= limit; q += p)
                is_comp[(size_t)q] = true;

    size_t new_cnt = 0;
    for (uint32_t i = 2; i <= limit; ++i)
        if (!is_comp[i]) ++new_cnt;

    uint32_t *new_arr = (uint32_t *)malloc(new_cnt * sizeof(uint32_t));
    if (!new_arr) { free(is_comp); return; }

    size_t idx = 0;
    for (uint32_t i = 2; i <= limit; ++i)
        if (!is_comp[i]) new_arr[idx++] = i;

    free(prime_list);
    prime_list  = new_arr;
    prime_count = new_cnt;
    free(is_comp);
}

/*  get_pfn – return the n‑th prime (1‑based).  */
static uint32_t get_pfn(idx_t n)
{
    if (n > 0 && prime_count < (size_t)n) {
        /*   Rough upper bound for the n‑th prime (Rosser):  n(ln n + ln ln n)  */
        uint32_t limit = 13;   /* 6th prime */
        if (n >= 6) {
            double nn  = (double)n;
            double est = nn * (log(nn) + log(log(nn)));
            limit = (uint32_t)(est + 10.0);
        }
        while (prime_count < (size_t)n) {
            build_primes(limit);
            if (prime_count >= (size_t)n) break;
            limit *= 2;
        }
    }
    return prime_list[(size_t)n - 1];
}

/*  Main sequence function  */
seq_t a(idx_t n)
{
    uint32_t p = get_pfn(n);           /* n‑th prime */
    double s  = sin((double)p);        /* sin(p)  */
    int    r  = (s >= 0.0) ? (int)(s + 0.5) : (int)(s - 0.5);  /* nearest integer */
    return (seq_t)r;                   /* cast to seq_t (works for all build modes) */
}
