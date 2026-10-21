/*
 * A098014  A098013/2.
 * https://oeis.org/A098014
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A098014: a(n) is the n‑th positive integer k such that there exist two consecutive primes p and q
 * with q = p + 2k and k itself is prime.  The sequence is obtained by sieving primes up to a
 * fixed limit, computing all such half–differences, and storing them in an array.  The function
 * a(idx_t n) returns the requested term. */
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/* ---------- Configuration ---------- */
#define LIMIT      20000000U          /* upper bound for the sieve   */
#define SEQ_CAP    200000U           /* number of sequence terms stored */

/* ---------- Static storage ---------- */
static bool          initialized = false;
static unsigned char *isprime    = NULL;   /* 1 : prime, 0 : composite */
static seq_t        *seq        = NULL;   /* precomputed sequence values */
static size_t        seq_len    = 0;       /* number of values stored */

/* ---------- Helper: build sieve and sequence ---------- */
static void init_sequence(void)
{
    /* Allocate prime bitmap */
    size_t i, j;
    size_t limit = (size_t)LIMIT;
    isprime = (unsigned char*)malloc(limit + 1);
    if (!isprime) return;                /* out‑of‑memory → function will return 0 */

    /* Initialise all entries as prime, then clear 0 and 1 */
    memset(isprime, 1, limit + 1);
    isprime[0] = isprime[1] = 0;

    /* Sieve of Eratosthenes */
    for (i = 2; i * i <= limit; ++i)
        if (isprime[i])
            for (j = i * i; j <= limit; j += i)
                isprime[j] = 0;

    /* Allocate array for the sequence */
    seq = (seq_t*)malloc(SEQ_CAP * sizeof(seq_t));
    if (!seq) { free(isprime); isprime = NULL; return; }

    /* Compute half‑differences of consecutive primes */
    idx_t prev = 0;                /* previous prime (0 indicates none yet) */
    seq_len = 0;
    for (size_t n = 2; n <= limit; ++n)
    {
        if (!isprime[n]) continue;         /* not a prime */

        if (prev != 0)                     /* we have a consecutive pair */
        {
            uint64_t diff = (uint64_t)n - (uint64_t)prev;
            if ((diff & 1ULL) == 0U)        /* even difference */
            {
                idx_t half = (idx_t)(diff >> 1);
                if (half <= limit && isprime[half])   /* half is also prime */
                {
                    if (seq_len < SEQ_CAP)
                        seq[seq_len++] = (seq_t)half;
                    else
                        break;                /* capacity reached */
                }
            }
        }
        prev = (idx_t)n;
    }

    initialized = true;
}

/* ---------- Public sequence function ---------- */
seq_t a(idx_t n)
{
    if (!initialized) init_sequence();

    if (n == 0) return 0;               /* undefined for 0 */
    if (n <= seq_len) return seq[n - 1];/* standard 1‑based indexing */
    return 0;                           /* beyond pre‑computed range */
}
