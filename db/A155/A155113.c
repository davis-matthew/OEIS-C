/*
 * A155113  Odd numbers n with odd largest digit of prime(n).
 * https://oeis.org/A155113
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the n-th odd integer k such that the largest decimal digit
   of prime(k) is odd. This implementation searches odd k in
   increasing order and keeps a small dynamic list of prime values. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Prime generation state */
static uint64_t *prime_array = NULL;
static size_t prime_cnt      = 0;    /* number of primes stored */
static size_t prime_cap      = 0;    /* capacity of prime_array */
static uint64_t cur_candidate = 2;   /* next number to test for primality */

/* Ensure at least 'need' primes are stored (prime(1) = 2). */
static void ensure_primes_upto_count(size_t need)
{
    if (prime_cnt >= need)
        return;

    /* Initial allocation */
    if (prime_cap == 0) {
        prime_cap = 16;
        prime_array = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
        if (!prime_array)
            exit(EXIT_FAILURE);
    }

    /* Generate primes until we have at least 'need' of them */
    while (prime_cnt < need) {
        uint64_t p = cur_candidate;
        bool is_p = true;

        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t d = prime_array[i];
            if (d * d > p)
                break;
            if (p % d == 0) {
                is_p = false;
                break;
            }
        }

        if (is_p) {
            /* Grow the array if needed */
            if (prime_cnt == prime_cap) {
                prime_cap <<= 1;
                uint64_t *tmp = (uint64_t *)realloc(prime_array,
                                                    prime_cap * sizeof(uint64_t));
                if (!tmp)
                    exit(EXIT_FAILURE);
                prime_array = tmp;
            }
            prime_array[prime_cnt++] = p;
        }

        /* Advance to the next candidate: skip even numbers */
        if (p == 2)
            cur_candidate = 3;
        else
            cur_candidate += 2;
    }
}

/* Return the n-th odd integer whose prime(n) has an odd largest digit. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    size_t found = 0;
    for (idx_t odd = 3; ; odd += 2) {            /* iterate odd indices */
        ensure_primes_upto_count((size_t)odd);
        seq_t p = (seq_t)prime_array[odd - 1];    /* prime(odd) */

        /* Find the largest decimal digit of p */
        unsigned largest = 0;
        seq_t tmp = p;
        while (tmp > 0) {
            uint64_t digit = (uint64_t)(tmp % 10);
            if (digit > largest)
                largest = (unsigned)digit;
            tmp /= 10;
        }

        if (largest & 1U) {                      /* largest digit is odd */
            ++found;
            if (found == (size_t)n)
                return (seq_t)odd;
        }
    }
}
