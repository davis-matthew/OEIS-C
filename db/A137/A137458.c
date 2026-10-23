/*
 * A137458  Prime(core(n)).
 * https://oeis.org/A137458
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = A000040(A007913(n)) – Prime of the squarefree part of n
 * Range:     u8 n <= 54, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/* Build a small prime table once. */
#define SIEVE_LIMIT 200000          /* enough for 10000th prime and squarefree parts up to 10000 */

static bool is_prime_arr[SIEVE_LIMIT + 1];
static uint32_t prime_list[SIEVE_LIMIT];
static size_t prime_count = 0;
static bool primes_ready = false;

/* Initialize the prime sieve and list. */
static void init_primes(void)
{
    if (primes_ready) return;
    primes_ready = true;

    for (size_t i = 0; i <= SIEVE_LIMIT; ++i) is_prime_arr[i] = true;
    if (SIEVE_LIMIT >= 0) is_prime_arr[0] = false;
    if (SIEVE_LIMIT >= 1) is_prime_arr[1] = false;

    for (size_t i = 2; i * i <= SIEVE_LIMIT; ++i) {
        if (is_prime_arr[i]) {
            for (size_t j = i * i; j <= SIEVE_LIMIT; j += i)
                is_prime_arr[j] = false;
        }
    }

    for (size_t i = 2; i <= SIEVE_LIMIT; ++i) {
        if (is_prime_arr[i]) {
            prime_list[prime_count++] = (uint32_t)i;
        }
    }
}

/* Compute the squarefree part of n (product of primes with odd exponent). */
static seq_t squarefree_part(idx_t n)
{
    seq_t result = 1;
    idx_t temp = n;

    for (size_t idx = 0; idx < prime_count; ++idx) {
        uint32_t p = prime_list[idx];
        if ((idx_t)p * (idx_t)p > temp) break;

        int parity = 0;
        while (temp % p == 0) {
            temp /= p;
            parity ^= 1;              /* flip between 0 and 1 */
        }
        if (parity) result *= (seq_t)p;
    }

    if (temp > 1) {                     /* remaining prime factor */
        result *= (seq_t)temp;
    }

    return result;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    init_primes();

    seq_t sf = squarefree_part(n);      /* squarefree part of n */
    /* sf is at least 1.  Convert sf-1 to a 0‑based index into prime_list. */
    size_t prime_index = (size_t)(sf - 1);
    /* We assume sf <= prime_count.  For values of n up to about 10000 this holds. */
    return (seq_t)prime_list[prime_index];
}
