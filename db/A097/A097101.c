/*
 * A097101  Numbers m that are the hypotenuse of exactly 7 distinct
 *          integer-sided right triangles, i.e., m^2 can be written as a sum
 *          of two squares in 7 ways.
 * https://oeis.org/A097101
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1260, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   prime_is_1mod4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n):   numbers that can be written as a sum of two squares in exactly
   7 different ways (unordered positive solutions).  The implementation
   uses the factorisation condition
      Π_{p≡1(4)} (2·e_p + 1) = 15
   where e_p is the exponent of p in the prime factorisation of m.
   All other prime factors are unrestricted.  The sequence is built
   by a simple sieve of eratosthenes with smallest‑prime‑factor storage
   and scanning integers incrementally until the required index is
   reached.  */

/* ----------  includes  ---------- */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/prime_is_1mod4.h"

/* ----------  global data  ---------- */
/* We do **not** re‑define seq_t or idx_t; they are supplied by the build. */

/* constant upper bound for the sieve. 10 000th term is below 10 000 000. */
static const uint32_t SIEVE_LIMIT = 10000000U;
/* smallest prime factor array – zero means prime */
static uint32_t *spf = NULL;
/* number of entries in the sieve */
static uint32_t spf_limit = 0;

/* array holding the sequence in ascending order */
static seq_t *seq_buf = NULL;      /* pointer to sequence values           */
static size_t seq_len = 0;         /* number of values currently stored    */
static size_t seq_cap = 0;         /* capacity of seq_buf                   */

/* next integer to be examined */
static uint32_t next_m = 1;

/* ----------  helper functions  ---------- */

static void init_sieve(void)
{
    if (spf_limit != 0) return;               /* already initialised */
    spf_limit = SIEVE_LIMIT;
    spf = (uint32_t *)malloc((size_t)(spf_limit + 1) * sizeof(uint32_t));
    if (!spf) return;                         /* unlikely failure */

    for (uint32_t i = 0; i <= spf_limit; ++i) spf[i] = 0;
    spf[0] = 0;
    spf[1] = 1;

    /* linear sieve for smallest prime factor */
    uint32_t *primes = (uint32_t *)malloc((size_t)(spf_limit / 10 + 10) * sizeof(uint32_t));
    if (!primes) return;
    size_t primes_cnt = 0;

    for (uint32_t i = 2; i <= spf_limit; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            if (primes_cnt < (size_t)(spf_limit / 10 + 9)) primes[primes_cnt++] = i;
        }
        for (size_t j = 0; j < primes_cnt && primes[j] <= spf[i] &&
             (uint64_t)i * primes[j] <= (uint64_t)spf_limit; ++j) {
            spf[i * primes[j]] = primes[j];
        }
    }

    free(primes);
}

/* grow the dynamic array for the sequence */
static void grow_seq_array(size_t needed)
{
    if (needed <= seq_cap) return;
    size_t new_cap = seq_cap == 0 ? 4096 : seq_cap * 2;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *new_buf = (seq_t *)realloc(seq_buf, new_cap * sizeof(seq_t));
    if (!new_buf) return;          /* allocation failure unlikely */
    seq_buf = new_buf;
    seq_cap = new_cap;
}

/* ----------  main sequence function  ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;     /* undefined, guard against misuse */

    /* ensure the sieve is ready */
    init_sieve();

    /* generate sequence values until we have at least n of them */
    while (seq_len < (size_t)n) {
        if (next_m > spf_limit) {
            /* our sieve range is insufficient – not expected for n ≤ 10000 */
            return 0;
        }

        uint32_t m = next_m++;
        uint32_t temp = m;
        uint32_t prod = 1;               /* Π (2·e + 1) over p≡1(4) */
        while (temp > 1) {
            uint32_t p = spf[temp];
            uint32_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            if (prime_is_1mod4(p)) {
                prod *= (2U * exp + 1U);
                if (prod > 15U) break;   /* early exit if product already too large */
            }
        }

        if (prod == 15U) {
            /* qualifies as a hypotenuse of exactly 7 right triangles */
            grow_seq_array(seq_len + 1);
            seq_buf[seq_len++] = (seq_t)m;
        }
    }

    return seq_buf[n - 1];
}
