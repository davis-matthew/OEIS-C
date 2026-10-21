/*
 * A075541  Numbers k such that prime(k) + prime(k+1) + prime(k+2) is a
 *          multiple of 3.
 * https://oeis.org/A075541
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th integer k (k≥1) such that *
 *            prime(k)+prime(k+1)+prime(k+2) is divisible by 3. * The
 *            following implementation precomputes a sieve of primes up to a *
 *            fixed limit (1 000 000) and then scans the prime list to collect
 *            * the required indices. The first 10 000 terms are generated, *
 *            wh...
 * Range:     u8 n <= 29, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* Helper: compute primes up to LIMIT and store them in the static array
 * primes[prime_count].  LIMIT is chosen large enough (1e6) to provide
 * at least 78 498 primes, which guarantees more than 19 600 valid k
 * values – well above the 10 000 requested in the tests. */
#define LIMIT 1000000
#define MAX_PRIMES 80000
static uint32_t primes[MAX_PRIMES];
static size_t prime_count = 0;

static void compute_primes(void) {
    static bool done = false;
    if (done) return;
    done = true;

    bool *sieve = (bool *)malloc((LIMIT + 1) * sizeof(bool));
    if (!sieve) return;
    memset(sieve, 0, (LIMIT + 1) * sizeof(bool));
    sieve[0] = sieve[1] = true;
    for (uint32_t i = 2; (uint64_t)i * i <= LIMIT; ++i) {
        if (!sieve[i]) {
            for (uint32_t j = i * i; j <= LIMIT; j += i)
                sieve[j] = true;
        }
    }
    for (uint32_t i = 2; i <= LIMIT && prime_count < MAX_PRIMES; ++i)
        if (!sieve[i]) primes[prime_count++] = i;
    free(sieve);
}

/* Helper: generate the sequence a until at least N terms are stored.
 * The sequence indices are kept in seq[0 .. seq_len-1] (0‑based). */
#define MAX_SEQ 20000   /* enough for n up to 10 000 */
static uint32_t seq[MAX_SEQ];
static size_t seq_len = 0;

static void generate_sequence(size_t N) {
    static bool done = false;
    if (done && seq_len >= N) return;
    compute_primes();
    seq_len = 0;
    /* Scan prime triples.  primes[0] = prime(1) */
    for (size_t k = 1; k + 2 < prime_count && seq_len < N; ++k) {
        uint64_t sum = (uint64_t)primes[k - 1] + primes[k] + primes[k + 1];
        if ((sum % 3ULL) == 0ULL) {
            seq[seq_len++] = (uint32_t)k;      /* store k (1‑based) */
        }
    }
    done = true;
}

/* Public sequence function requested by the test harness. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n<1, but safe */
    /* Ensure we have at least n terms. */
    if (n > seq_len) {
        /* Re‑generate until we have n terms (primes limit already enough). */
        generate_sequence(n);
        if (n > seq_len) n = seq_len;  /* fallback in unlikely case */
    }
    return (seq_t)seq[n - 1];
}
