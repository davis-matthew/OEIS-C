/*
 * A124622  Smallest odd interprime divisible by n-th odd prime.
 * https://oeis.org/A124622
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (prime(A126233(n)+1)+prime(A126233(n)))/2
 * Range:     u8 n <= 5, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Dynamically stored list of primes (primes[0] = 2, primes[1] = 3, …) */
static uint64_t *primes   = NULL;  /* array of primes, 0‑based index */
static size_t     primes_cnt = 0;  /* number of primes stored */
static size_t     primes_cap = 0;  /* allocated capacity of the array  */
static uint64_t   next_candidate = 3;  /* next odd number to test for primality */

/* Ensure that at least `need` primes are available (indices 0 … need‑1). */
static void ensure_primes(size_t need)
{
    if (primes_cnt >= need)
        return;

    if (!primes) {
        primes_cap = 256;
        primes = (uint64_t *)malloc(primes_cap * sizeof(uint64_t));
        if (!primes)
            exit(EXIT_FAILURE);
        primes[0] = 2;
        primes_cnt = 1;
        next_candidate = 3;
    }

    while (primes_cnt < need) {
        uint64_t p = next_candidate;
        bool is_prime = true;
        for (size_t i = 0; i < primes_cnt; ++i) {
            uint64_t prime = primes[i];
            if (prime * prime > p)
                break;
            if (p % prime == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            if (primes_cnt >= primes_cap) {
                primes_cap *= 2;
                uint64_t *newp = (uint64_t *)realloc(primes, primes_cap * sizeof(uint64_t));
                if (!newp)
                    exit(EXIT_FAILURE);
                primes = newp;
            }
            primes[primes_cnt++] = p;
        }
        next_candidate += 2;
    }
}

/* OEIS sequence A124622 */
seq_t a(idx_t n)
{
    if (n == 0)            /* offset 1: values for n < 1 are undefined */
        return 0;

    /* nth odd prime is primes[n] (primes[0] = 2). */
    ensure_primes((size_t)n + 1);
    seq_t odd_prime = (seq_t)primes[(size_t)n];

    for (size_t i = 2; ; ++i) {                /* i is the index of the first prime in the pair */
        /* We need primes p_i and p_{i+1}. */
        ensure_primes(i + 2);
        seq_t pi   = (seq_t)primes[i - 1];
        seq_t pip1 = (seq_t)primes[i];

        seq_t sum = pi + pip1;
        /* Keep only pairs whose average is odd: (pi + pip1) ≡ 2 (mod 4). */
        if ((sum & (seq_t)0x3) != (seq_t)0x2)
            continue;

        seq_t avg = sum >> 1;      /* (p_i + p_{i+1}) / 2  */
        if (avg % odd_prime == 0)
            return avg;
    }
}
