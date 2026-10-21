/*
 * A083723  a(n) = (prime(n)+1)*n - 1.
 * https://oeis.org/A083723
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (prime(n)+1)*n - 1
 * Range:     u8 n <= 9, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* prime_list holds primes in 1‑based order: prime_list[0] = 2. */
static uint32_t *prime_list = NULL;
static size_t prime_cnt  = 0;   /* number of primes stored       */
static size_t prime_cap  = 0;   /* allocated capacity            */

/* Expand prime_list so that it contains at least 'target' primes. */
static void ensure_primes(idx_t target)
{
    if (prime_cnt >= (size_t)target)
        return; /* already have enough primes */

    uint64_t candidate = (prime_cnt == 0) ? 2 : (uint64_t)prime_list[prime_cnt - 1] + 1;

    while (prime_cnt < (size_t)target) {
        /* Treat 2 specially */
        if (candidate == 2) {
            /* grow array if necessary */
            if (prime_cnt == prime_cap) {
                prime_cap = prime_cap ? prime_cap * 2 : 16;
                uint32_t *tmp = (uint32_t *)realloc(prime_list, prime_cap * sizeof(uint32_t));
                if (!tmp) exit(1);
                prime_list = tmp;
            }
            prime_list[prime_cnt++] = 2;
            candidate = 3;          /* next odd number */
            continue;
        }

        /* Skip even numbers */
        if ((candidate & 1) == 0) {
            candidate++;
            continue;
        }

        /* Trial division by known primes */
        int is_prime = 1;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint32_t p = prime_list[i];
            if ((uint64_t)p * (uint64_t)p > candidate)
                break;
            if (candidate % p == 0) {
                is_prime = 0;
                break;
            }
        }

        if (is_prime) {
            /* grow array if necessary */
            if (prime_cnt == prime_cap) {
                prime_cap = prime_cap ? prime_cap * 2 : 16;
                uint32_t *tmp = (uint32_t *)realloc(prime_list, prime_cap * sizeof(uint32_t));
                if (!tmp) exit(1);
                prime_list = tmp;
            }
            prime_list[prime_cnt++] = (uint32_t)candidate;
        }

        candidate += 2;  /* advance to next odd candidate */
    }
}

seq_t a(idx_t n)
{
    /* Sequence definition starts at n = 1. */
    if (n < 1)
        return (seq_t)0;

    ensure_primes(n);

    uint32_t p = prime_list[n - 1];          /* prime(n) */
    seq_t sn = (seq_t)n;
    seq_t sp = (seq_t)p;

    return sn * (sp + (seq_t)1) - (seq_t)1;
}
