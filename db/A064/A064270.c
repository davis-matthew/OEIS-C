/*
 * A064270  Primes of the form prime(k) - k; or primes arising in A014689.
 * https://oeis.org/A064270
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the n-th prime of the form prime(k) - k (OEIS A064270)
 * Range:     u8 n <= 15, u16 n <= 702, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/*---------------------------------------------------------------------*/
/*  Static data structures                                            */
/*---------------------------------------------------------------------*/
static bool initialized = false;

/* sieve of Eratosthenes for primality test up to SIEVE_LIMIT          */
static const idx_t SIEVE_LIMIT = 10000000;      /* 10 million */
static unsigned char *is_prime_sieve = NULL;

/* list of prime numbers up to SIEVE_LIMIT                            */
static idx_t *prime_list = NULL;
static size_t prime_list_capacity = 0;
static size_t prime_count = 0;

/* generated sequence values                                         */
static seq_t *seq_array = NULL;
static size_t seq_array_capacity = 0;
static size_t seq_array_len = 0;

/* next k to be examined (1‑based)                                    */
static idx_t next_k = 1;

/*---------------------------------------------------------------------*/
/*  Local helper functions                                           */
/*---------------------------------------------------------------------*/
static void init(void)
{
    if (initialized) return;
    initialized = true;

    /* --- build the sieve ------------------------------------------ */
    is_prime_sieve = (unsigned char*)malloc((size_t)SIEVE_LIMIT + 1);
    if (!is_prime_sieve) exit(1);
    /* 1 = probably prime, 0 = composite */
    memset(is_prime_sieve, 1, (size_t)SIEVE_LIMIT + 1);
    is_prime_sieve[0] = 0;
    is_prime_sieve[1] = 0;

    for (idx_t i = 2; i * i <= SIEVE_LIMIT; ++i) {
        if (is_prime_sieve[i]) {
            for (idx_t j = i * i; j <= SIEVE_LIMIT; j += i)
                is_prime_sieve[j] = 0;
        }
    }

    /* --- produce the list of primes up to SIEVE_LIMIT -------------- */
    prime_list_capacity = SIEVE_LIMIT / 10 + 10;  /* rough upper bound */
    prime_list = (idx_t*)malloc(prime_list_capacity * sizeof(idx_t));
    if (!prime_list) exit(1);

    prime_count = 0;
    for (idx_t i = 2; i <= SIEVE_LIMIT; ++i) {
        if (is_prime_sieve[i]) {
            if (prime_count >= prime_list_capacity) {
                prime_list_capacity *= 2;
                prime_list = (idx_t*)realloc(prime_list,
                                            prime_list_capacity * sizeof(idx_t));
                if (!prime_list) exit(1);
            }
            prime_list[prime_count++] = i;
        }
    }
}

/* Ensure that seq_array contains at least n terms.                  */
static void ensure_sequence_upto(idx_t n)
{
    if (!initialized) init();

    while (seq_array_len < n && next_k <= (idx_t)prime_count) {
        idx_t pk = prime_list[next_k - 1];   /* prime(k) */
        idx_t val = pk - next_k;            /* candidate prime   */

        if (val >= 2 && val <= SIEVE_LIMIT && is_prime_sieve[val]) {
            if (seq_array_len >= seq_array_capacity) {
                seq_array_capacity = seq_array_capacity ? seq_array_capacity * 2 : 16;
                seq_array = (seq_t*)realloc(seq_array,
                                            seq_array_capacity * sizeof(seq_t));
                if (!seq_array) exit(1);
            }
            seq_array[seq_array_len++] = (seq_t)val;
        }
        ++next_k;
    }
}

/*---------------------------------------------------------------------*/
/*  Required function                                                */
/*---------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* undefined for offset 0 */
    ensure_sequence_upto(n);
    if (n - 1 < seq_array_len)
        return seq_array[n - 1];
    return (seq_t)0;                       /* should not happen for valid n */
}
