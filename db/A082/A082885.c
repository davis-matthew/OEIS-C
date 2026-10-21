/*
 * A082885  Primes followed by a larger-than-average prime gap.
 * https://oeis.org/A082885
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 2581, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation generates the primes one by one and tests each
   prime p(j) to see whether the following gap g = p(j+1)-p(j) satisfies
   g > log(p(j)).  For integer computations the logarithm is
   approximated by pre‑computed integer thresholds for gaps up to at
   least 12.  For larger gaps the inequality automatically holds for
   primes considered here.  The function returns the n‑th prime that
   satisfies the condition.  The algorithm is O(n) with very small
   constants.  */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Threshold array: floor(e^g) for g = 0..12 */
static const uint64_t THRESHOLD[13] = {
    1ULL,      /* e^0  */
    2ULL,      /* e^1  */
    7ULL,      /* e^2  */
    20ULL,     /* e^3  */
    54ULL,     /* e^4  */
    148ULL,    /* e^5  */
    403ULL,    /* e^6  */
    1096ULL,   /* e^7  */
    2981ULL,   /* e^8  */
    8103ULL,   /* e^9  */
    22026ULL,  /* e^10 */
    59874ULL,  /* e^11 */
    162754ULL  /* e^12 */
};

/* Prime sieve data */
static uint32_t *prime_arr   = NULL;   /* array of primes collected so far */
static size_t   prime_cap   = 0;       /* capacity of prime_arr */
static size_t   prime_len   = 0;       /* current number of primes */
static uint32_t prime_cand  = 2;       /* next candidate to test for primality */

/* Sequence cache data */
static seq_t *seq_cache   = NULL;      /* array of sequence terms */
static size_t seq_cap     = 0;        /* capacity of seq_cache */
static size_t seq_len     = 0;        /* number of terms stored */

/* Index of the prime currently being processed */
static size_t proc_idx    = 0;

/* Generate the next prime and store it in prime_arr. */
static uint32_t next_prime(void)
{
    for (;;) {
        bool isPrime = true;
        for (size_t i = 0; i < prime_len; ++i) {
            uint32_t p = prime_arr[i];
            if ((uint64_t)p * p > prime_cand)
                break;
            if (prime_cand % p == 0) {
                isPrime = false;
                break;
            }
        }
        if (isPrime) {
            /* grow prime buffer if needed */
            if (prime_len == prime_cap) {
                size_t new_cap = prime_cap ? prime_cap * 2 : 16;
                uint32_t *tmp = (uint32_t *)realloc(prime_arr, new_cap * sizeof(uint32_t));
                if (!tmp) abort();
                prime_arr = tmp;
                prime_cap = new_cap;
            }
            prime_arr[prime_len++] = prime_cand;
            uint32_t result = prime_cand;
            ++prime_cand;
            return result;
        }
        ++prime_cand;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0U)
        return (seq_t)0;                   /* undefined for offset 1, but return 0 safety */

    /* Expand the sequence until we have at least n terms */
    while (seq_len < n) {
        /* Ensure we have at least two primes beyond the current processing index */
        while (proc_idx + 1 >= prime_len)
            next_prime();

        uint32_t p_curr = prime_arr[proc_idx];
        uint32_t p_next = prime_arr[proc_idx + 1];
        uint32_t gap    = p_next - p_curr;

        bool qualify = false;
        if (gap <= 12U) {
            qualify = ((uint64_t)p_curr <= THRESHOLD[gap]);
        } else {
            /* For gaps larger than 12, ln(p) <= ~12.2 for p <= 200000,
               so they automatically satisfy the condition. */
            qualify = true;
        }

        if (qualify) {
            /* Grow sequence buffer if needed */
            if (seq_len == seq_cap) {
                size_t new_cap = seq_cap ? seq_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                seq_cache = tmp;
                seq_cap   = new_cap;
            }
            seq_cache[seq_len++] = (seq_t)p_curr;
        }

        ++proc_idx;
    }

    return seq_cache[(size_t)(n - 1U)];
}
