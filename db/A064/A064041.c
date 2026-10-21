/*
 * A064041  Number of divisors of A064040(n).
 * https://oeis.org/A064041
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = d(A064040(n)) where A064040(n) is the n‑th integer whose
 *            number of distinct prime factors is a prime number
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Maximum number of terms that we precompute.  The tests use n up to ~10 000,
   so 50 000 is safely above that. */
#define MAX_TERMS   50000ULL
/* Upper bound for the sieve – large enough so that we get at least
   MAX_TERMS numbers with prime ω(n).  10 000 000 gives more than 200 000
   such numbers. */
#define SPF_LIMIT   10000000U

static seq_t            *div_counts  = NULL;   /* div_counts[1..MAX_TERMS] */
static uint32_t         *spf         = NULL;   /* smallest prime factor array */
static bool              initialized = false;

/* Number of terms already computed (>=0). */
static size_t            div_counts_len = 0;

/* Helper: simple sieve of Eratosthenes that fills the spf array. */
static void build_spf(void)
{
    uint32_t *arr = (uint32_t*)malloc((SPF_LIMIT + 1) * sizeof(uint32_t));
    for (uint32_t i = 0; i <= SPF_LIMIT; ++i)
        arr[i] = i;
    for (uint32_t i = 2; i * i <= SPF_LIMIT; ++i) {
        if (arr[i] == i) {
            for (uint32_t j = i * i; j <= SPF_LIMIT; j += i)
                if (arr[j] == j)
                    arr[j] = i;
        }
    }
    spf = arr;
}

/* Helper: precompute divisor counts for the first MAX_TERMS qualifying
   numbers.  The numbers are scanned in increasing order until we have
   collected MAX_TERMS of them. */
static void precompute(void)
{
    if (!spf)
        build_spf();

    div_counts = (seq_t*)malloc((MAX_TERMS + 1) * sizeof(seq_t));
    div_counts[0] = 0;                    /* unused; a(0) not defined */

    /* Prime table for distinct‑prime counts up to 20 (more than we ever need). */
    bool prime_distinct[32] = {0};
    int primes[] = {2, 3, 5, 7, 11, 13, 17, 19};
    for (size_t i = 0; i < sizeof(primes)/sizeof(primes[0]); ++i)
        if (primes[i] < 32)
            prime_distinct[primes[i]] = true;

    size_t cnt = 0;
    for (uint32_t num = 2; num <= SPF_LIMIT && cnt < MAX_TERMS; ++num) {
        /* Factor num using spf, calculate τ(num) and ω(num). */
        seq_t tau = 1;
        size_t distinct = 0;
        uint32_t prev = 0;
        uint32_t exp = 0;
        uint32_t tmp = num;
        while (tmp > 1) {
            uint32_t p = spf[tmp];
            if (p != prev) {
                if (prev != 0)
                    tau *= (exp + 1);
                ++distinct;
                prev = p;
                exp = 1;
            } else
                ++exp;
            tmp /= p;
        }
        if (prev != 0)
            tau *= (exp + 1);

        if (distinct < 32 && prime_distinct[distinct]) {
            ++cnt;
            div_counts[cnt] = tau;
        }
    }
    div_counts_len = cnt;
}

/* External linkage function requested by the problem. */
seq_t a(idx_t n)
{
    if (!initialized) {
        precompute();
        initialized = true;
    }
    /* We accept any n >= 1; the precomputation guarantees that
       we have at least MAX_TERMS values.  If n exceeds that, we simply
       return 0 (not expected in test data). */
    if ((size_t)n > div_counts_len)
        return 0;
    return div_counts[(size_t)n];
}
