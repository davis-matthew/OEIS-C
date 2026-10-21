/*
 * A083681  Sum of divisors of semiprimes.
 * https://oeis.org/A083681
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 57, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   A083681 – Sum of divisors of semiprimes
   Formula:  a(n) = σ(p·q) where p·q is the n‑th semiprime;
   σ(p·q) = (p + 1)(q + 1) for distinct primes p ≠ q,
   σ(p²) = 1 + p + p² for p = q.
   The code builds the list of semiprimes in order and stores their σ
   values.  It resizes the storage as larger n's are requested.
*/

#include <stdlib.h>
#include <stdint.h>
#include <math.h>

/* ----------  data structures  ---------- */
typedef struct {
    uint64_t prod;   /* product p*q */
    seq_t    sigma;  /* σ(p*q) */
} Semi;

/* ----------  static storage  ---------- */
static uint64_t *primes      = NULL; /* prime list */
static size_t   primes_cnt   = 0;
static size_t   primes_cap   = 0;

static Semi *semis       = NULL; /* sorted semiprimes */
static size_t  semis_cnt = 0;
static size_t  semis_cap = 0;

static uint64_t current_limit = 0; /* largest prime limit used */

/* ----------  helpers for dynamic arrays  ---------- */
static void ensure_primes_cap(size_t needed)
{
    if (primes_cap >= needed) return;
    size_t new_cap = primes_cap ? primes_cap * 2 : 16;
    if (new_cap < needed) new_cap = needed;
    uint64_t *tmp = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
    if (!tmp) abort();
    primes = tmp;
    primes_cap = new_cap;
}

static void ensure_semis_cap(size_t needed)
{
    if (semis_cap >= needed) return;
    size_t new_cap = semis_cap ? semis_cap * 2 : 16;
    if (new_cap < needed) new_cap = needed;
    Semi *tmp = (Semi *)realloc(semis, new_cap * sizeof(Semi));
    if (!tmp) abort();
    semis = tmp;
    semis_cap = new_cap;
}

/* ----------  prime sieve  ---------- */
static void build_primes(uint64_t limit)
{
    size_t lim = (size_t)limit;
    uint8_t *is_comp = (uint8_t *)calloc(lim + 1, 1);
    if (!is_comp) abort();

    for (size_t i = 2; i * i <= lim; ++i) {
        if (!is_comp[i]) {
            for (size_t j = i * i; j <= lim; j += i)
                is_comp[j] = 1;
        }
    }

    primes_cnt = 0;
    ensure_primes_cap(lim / 10 + 10); /* rough initial capacity */

    for (size_t i = 2; i <= lim; ++i) {
        if (!is_comp[i]) {
            ensure_primes_cap(primes_cnt + 1);
            primes[primes_cnt++] = i;
        }
    }
    free(is_comp);
}

/* ----------  comparison for qsort  ---------- */
static int cmp_semis(const void *a, const void *b)
{
    const Semi *sa = (const Semi *)a;
    const Semi *sb = (const Semi *)b;
    if (sa->prod < sb->prod) return -1;
    if (sa->prod > sb->prod) return 1;
    return 0;
}

/* ----------  estimate upper bound for n‑th semiprime  ---------- */
static uint64_t estimate_limit(uint64_t n)
{
    if (n < 10) return 20;
    double nd = (double)n;
    double est = nd * (log(nd) + log(log(nd)));
    est *= 1.7;                     /* safety factor */
    return (uint64_t)(est + 10);
}

/* ----------  generate semiprimes up to required count  ---------- */
static void generate_up_to(uint64_t needed)
{
    if (semis_cnt >= needed) return; /* already enough */

    uint64_t upper = estimate_limit(needed);
    while (1) {
        if (upper > current_limit) {
            build_primes(upper);
            current_limit = upper;
        }

        /* regenerate semiprimes list */
        semis_cnt = 0;
        ensure_semis_cap(upper * 2);   /* generous capacity */

        for (size_t i = 0; i < primes_cnt; ++i) {
            uint64_t p = primes[i];
            for (size_t j = i; j < primes_cnt; ++j) {
                uint64_t q = primes[j];
                uint64_t prod = p * q;
                if (prod > upper) break;
                ensure_semis_cap(semis_cnt + 1);
                semis[semis_cnt].prod = prod;
                if (p == q) {
                    semis[semis_cnt].sigma = (seq_t)p * (seq_t)p + (seq_t)p + 1;
                } else {
                    semis[semis_cnt].sigma = ((seq_t)p + 1) * ((seq_t)q + 1);
                }
                ++semis_cnt;
            }
        }

        qsort(semis, semis_cnt, sizeof(Semi), cmp_semis);

        if (semis_cnt >= needed) break;
        upper *= 2;   /* not enough; double the search bound */
    }
}

/* ----------  public function  ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* safety for undefined n=0 */
    generate_up_to((uint64_t)n);
    return semis[(size_t)(n - 1)].sigma;
}
