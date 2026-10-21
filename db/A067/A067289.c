/*
 * A067289  Numbers k such that the number of divisors of k is
 *          floor(prime(k)/k).
 * https://oeis.org/A067289
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 3030, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = the n-th integer k such that d(k) = floor(prime(k)/k),
 *               where d(k) is the number of divisors of k and prime(k) is
 *               the k-th prime.
 *               (Sequence A067289, offset 1)
 *
 * This implementation dynamically searches increasing integers k,
 * stores the qualifying terms in a small array of uint64_t, and
 * returns the k-th term on demand.  All integer arithmetic is
 * performed in uint64_t or lower to avoid overflow; the resulting
 * value is cast to seq_t on return.
 *
 * The routine builds a small prime sieve on demand; it grows
 * only when the next required prime index exceeds the current
 * sieve limit.  Divisor counts are computed by trial division
 * using the generated primes.
 *
 * The code is compliant with C11 and C++17 (all malloc/realloc
 * calls are explicitly cast).  It does not use any of the
 * helper libraries listed in the problem statement.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ---------- global prime table ------------------------------------ */
static uint32_t *primes = NULL;          /* list of primes (uint32_t)      */
static size_t   primes_len  = 0;         /* number of primes stored       */
static size_t   primes_cap  = 0;         /* allocated capacity            */
static size_t   prime_limit  = 0;        /* highest rational limit sieved  */

/* ---------- global term table ------------------------------------- */
static uint64_t *terms_u64 = NULL;      /* stored terms as uint64_t      */
static size_t   terms_len  = 0;          /* number of terms stored        */
static size_t   terms_cap  = 0;          /* allocated capacity            */

static idx_t   last_checked_k = 0;       /* last candidate examined       */

/* ---------- utilities --------------------------------------------- */
/* Rebuild the prime sieve up to 'limit' (inclusive).  All older data
   is discarded.  The function is called only when the current sieve
   no longer contains enough primes. */
static void rebuild_primes_upto(size_t limit)
{
    /* free old data */
    free(primes);
    primes = (uint32_t*)malloc((limit / 10 + 10) * sizeof(uint32_t));
    primes_len = 0;

    uint8_t *sieve = (uint8_t*)calloc(limit + 1, 1);   /* 0 = prime     */
    for (size_t i = 2; i <= limit; ++i) {
        if (!sieve[i]) {
            if (primes_len + 1 > (limit / 10 + 10)) {
                primes_cap = primes_len + 1;
                primes = (uint32_t*)realloc(primes, primes_cap * sizeof(uint32_t));
            }
            primes[primes_len++] = (uint32_t)i;
            if ((uint64_t)i * i <= limit) {
                for (uint64_t j = (uint64_t)i * i; j <= limit; j += i)
                    sieve[(size_t)j] = 1;
            }
        }
    }
    free(sieve);
    prime_limit = limit;
}

/* Ensure that the prime table contains at least 'cnt' primes.
   If not, increase the sieve size until it does. */
static void ensure_primes_at_least(idx_t cnt)
{
    if ((size_t)cnt <= primes_len)
        return;

    size_t target = (prime_limit ? prime_limit * 2 : 2000000);
    while ((size_t)cnt > primes_len) {
        rebuild_primes_upto(target);
        target *= 2;
    }
}

/* Number of divisors of a 64-bit integer using trial division
   by the precomputed primes. */
static uint32_t divisor_count_u64(uint64_t n)
{
    uint32_t res = 1;
    uint64_t temp = n;
    for (size_t i = 0; i < primes_len && (uint64_t)primes[i] * primes[i] <= temp; ++i) {
        if (temp % primes[i] == 0) {
            uint32_t exp = 0;
            while (temp % primes[i] == 0) {
                temp /= primes[i];
                ++exp;
            }
            res *= (exp + 1);
        }
    }
    if (temp > 1)  /* remaining prime factor */
        res *= 2;
    return res;
}

/* Append a term to the term table, expanding if necessary. */
static void push_term(seq_t val)
{
    if (terms_len == terms_cap) {
        terms_cap = terms_cap ? terms_cap * 2 : 1024;
        terms_u64 = (uint64_t*)realloc(terms_u64, terms_cap * sizeof(uint64_t));
    }
    terms_u64[terms_len++] = (uint64_t)val;
}

/* Generate terms until at least 'need' terms are stored. */
static void generate_up_to(idx_t need)
{
    if (primes_len == 0)
        rebuild_primes_upto(2000000);  /* initial sieve */

    while (terms_len < need) {
        ++last_checked_k;  /* candidate k */

        /* Ensure primes up to last_checked_k-th prime */
        ensure_primes_at_least(last_checked_k);

        /* floor( prime(k) / k ) */
        uint64_t pk = (uint64_t)primes[last_checked_k - 1];
        uint32_t floor_val = (uint32_t)(pk / (uint64_t)last_checked_k);

        uint32_t d = divisor_count_u64((uint64_t)last_checked_k);

        if (d == floor_val)
            push_term((seq_t)last_checked_k);
    }
}

/* ---------- public function ---------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)  /* Undefined for offset 0 */
        return (seq_t)0;

    generate_up_to(n);
    return (seq_t)terms_u64[n - 1];
}
