/*
 * A076299  Numbers k such that prime(k) + s*k is prime, s=4.
 * https://oeis.org/A076299
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 82, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A076299
 *
 * Definition:  a(n) is the n‑th integer k ≥ 1 such that
 *              prime(k) + 4·k is prime.
 *              Here prime(k) denotes the k‑th prime (prime(1)=2).
 *
 * Algorithm:  On the first call we sieve all primes up to a
 *              fixed limit (20 000 000).  From the sieve we build
 *              a list of all primes.  We then scan the integers
 *              k in order and test whether prime(k)+4·k is prime
 *              using the sieve array.  Whenever the test is true
 *              we append k to a dynamic array holding the
 *              sequence values.  Further calls continue from the
 *              last k that was examined, so the function works
 *              for calls in any order.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ------------------------------------------------------------
 * Configuration
 * ------------------------------------------------------------ */
#define SIEVE_LIMIT 20000000ULL      /* enough for n ≈ 10 000 */

/* ------------------------------------------------------------
 * Static state
 * ------------------------------------------------------------ */
static bool initialized = false;

/*  Sieve and prime list  */
static uint8_t *is_prime = NULL;   /* is_prime[i] == 1  ⇔  i is prime      */
static int   *prime_list = NULL;   /* list of primes                  */
static size_t primes_cnt = 0;      /* number of primes known          */
static size_t primes_cap = 0;      /* capacity of prime_list array    */

/*  Sequence storage  */
static seq_t *seq_array = NULL;    /* a(n) values   (seq_array[0] = a(1))   */
static size_t seq_len = 0;         /* number of values stored */;
static size_t seq_cap = 0;         /* capacity of seq_array */

/*  Progress state  */
static idx_t current_k = 0;        /* last k examined (1‑based) */

/* ------------------------------------------------------------
 * Helper: grow prime list
 * ------------------------------------------------------------ */
static void ensure_prime_cap(size_t min_cap)
{
    if (prime_list && min_cap <= primes_cap) return;
    size_t new_cap = primes_cap ? primes_cap * 2 : 1000;
    while (new_cap < min_cap) new_cap <<= 1;
    int *new_list = (int *)realloc(prime_list, new_cap * sizeof(int));
    if (!new_list) abort();                /* out of memory */
    prime_list = new_list;
    primes_cap = new_cap;
}

/* ------------------------------------------------------------
 * Helper: grow sequence array
 * ------------------------------------------------------------ */
static void ensure_seq_cap(size_t min_cap)
{
    if (seq_array && min_cap <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 10;
    while (new_cap < min_cap) new_cap <<= 1;
    seq_t *new_seq = (seq_t *)realloc(seq_array, new_cap * sizeof(seq_t));
    if (!new_seq) abort();                /* out of memory */
    seq_array = new_seq;
    seq_cap = new_cap;
}

/* ------------------------------------------------------------
 * Sieve of Eratosthenes up to SIEVE_LIMIT
 * ------------------------------------------------------------ */
static void init_sieve(void)
{
    is_prime = (uint8_t *)malloc((SIEVE_LIMIT + 1) * sizeof(uint8_t));
    if (!is_prime) abort();

    /* initialise all as prime */
    for (size_t i = 0; i <= SIEVE_LIMIT; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;

    for (size_t i = 2; i * i <= SIEVE_LIMIT; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= SIEVE_LIMIT; j += i)
                is_prime[j] = 0;
        }
    }

    /* build list of primes */
    primes_cap = 1000000;    /* start capacity */
    prime_list = (int *)malloc(primes_cap * sizeof(int));
    if (!prime_list) abort();

    for (size_t i = 2; i <= SIEVE_LIMIT; ++i) {
        if (is_prime[i]) {
            if (primes_cnt >= primes_cap) ensure_prime_cap(primes_cnt + 1);
            prime_list[primes_cnt++] = (int)i;
        }
    }
}

/* ------------------------------------------------------------
 * Main sequence function
 * ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (!initialized) {
        init_sieve();
        initialized = true;
    }

    /* build the sequence up to the requested term */
    while (seq_len < (size_t)n) {
        ++current_k;                     /* next k to test (1‑based) */

        /* need a larger prime list?  In practice the chosen SIEVE_LIMIT
           is sufficient for the required n.  If it is exceeded we abort
           because dynamic extension is not implemented. */
        if ((size_t)current_k > primes_cnt) abort();

        int pk = prime_list[current_k - 1];
        unsigned long long test_val = (unsigned long long)pk + 4ULL * (unsigned long long)current_k;

        if (test_val <= SIEVE_LIMIT && is_prime[(size_t)test_val]) {
            ensure_seq_cap(seq_len + 1);
            seq_array[seq_len++] = (seq_t)current_k;
        }
    }

    return seq_array[n - 1];   /* seq_array is 0‑based but a(1) is first term */
}
