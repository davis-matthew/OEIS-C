/*
 * A066513  Numbers n such that f(n) = f(n-1) - f(n-2) where f is the prime
 *          gap function given by f(m) = prime(m+1) - prime(m) and prime(m)
 *          denotes the m-th prime.
 * https://oeis.org/A066513
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   append_idx, append_uint32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS Sequence A066513 – indices n such that
 *  f(n) = f(n‑1) – f(n‑2), where f(k) is the k‑th prime gap
 *  (f(k) = prime(k+1) – prime(k)).
 *
 *  Implementation: 1) Generate all primes up to a large fixed limit
 *  (10 000 000) with a simple Eratosthenes sieve.
 *  2) Produce the list of prime gaps.
 *  3) Find all indices n ≥ 3 with f(n) = f(n‑1) – f(n‑2);
 *     store them in a static array.
 *  4) a(idx_t n) returns the n‑th element of that array.
 *
 *  The code is O(limit log log limit) and uses only
 *  integer arithmetic.  It works with the generic seq_t/idx_t types
 *  supplied by the build and compiles as C11 and C++17.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/append_idx.h"
#include "../utils/append_uint32.h"

/*---------------------  static data  ---------------------*/

static int initialized = 0;

static const size_t SIEVE_LIMIT = 10000000;  /* 10 M */

static uint8_t *is_comp = NULL;   /* sieve array */
static uint32_t *prime_buf = NULL;   /* primes list */
static size_t prime_cnt = 0;
static size_t prime_cap = 0;

static uint32_t *gap_buf = NULL;     /* prime gaps */
static size_t gap_cnt = 0;
static size_t gap_cap = 0;

static idx_t *term_buf = NULL;      /* indices satisfying recurrence */
static size_t term_len = 0;
static size_t term_cap = 0;

/*---------------------  helpers  ---------------------*/

/*---------------------  initialization  ---------------------*/

static void init_sequence(void)
{
    if (initialized) return;
    initialized = 1;

    /* 1. Sieve of Eratosthenes up to SIEVE_LIMIT */
    is_comp = (uint8_t*)calloc(SIEVE_LIMIT + 1, 1);
    if (!is_comp) return;  /* memory failure – should not happen */

    /* simple sqrt without <math.h> */
    size_t sq_root = 1;
    while ((sq_root + 1) * (sq_root + 1) <= SIEVE_LIMIT) ++sq_root;

    for (size_t i = 2; i <= sq_root; ++i)
    {
        if (!is_comp[i])
        {
            for (size_t j = i * i; j <= SIEVE_LIMIT; j += i)
                is_comp[j] = 1;
        }
    }

    /* 2. Collect primes */
    prime_cap = 1024;
    prime_buf = (uint32_t*)malloc(prime_cap * sizeof(uint32_t));
    for (size_t i = 2; i <= SIEVE_LIMIT; ++i)
    {
        if (!is_comp[i])
            append_uint32(&prime_buf, &prime_cnt, &prime_cap, (uint32_t)i);
    }

    /* 3. Build gaps array */
    gap_cap = prime_cnt > 0 ? prime_cnt : 1;
    gap_buf = (uint32_t*)malloc(gap_cap * sizeof(uint32_t));
    for (size_t i = 1; i < prime_cnt; ++i)
    {
        uint32_t g = prime_buf[i] - prime_buf[i - 1];
        append_uint32(&gap_buf, &gap_cnt, &gap_cap, g);
    }

    /* 4. Find indices n satisfying f(n) = f(n-1) – f(n-2) */
    term_cap = 1024;
    term_buf = (idx_t*)malloc(term_cap * sizeof(idx_t));
    for (size_t i = 2; i < gap_cnt; ++i) /* i corresponds to n = i+1 */
    {
        int32_t diff = (int32_t)gap_buf[i - 1] - (int32_t)gap_buf[i - 2];
        if ((int32_t)gap_buf[i] == diff)
            append_idx(&term_buf, &term_len, &term_cap, (idx_t)(i + 1));
    }

    /* 5. Free sieve (no longer needed) */
    free(is_comp);
}

/*---------------------  public function  ---------------------*/

seq_t a(idx_t n)
{
    init_sequence();                /* ensure data is ready */

    if (n == (idx_t)0 || n > (idx_t)term_len)
        return (seq_t)0;           /* out of range – caller should not ask for it */

    return (seq_t)term_buf[n - 1];  /* sequence is 1‑based */
}
