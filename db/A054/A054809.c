/*
 * A054809  Second term of strong prime 5-tuples: p(m)-p(m-1) > p(m+1)-p(m) >
 *          p(m+2)-p(m+1) > p(m+3)-p(m+2).
 * https://oeis.org/A054809
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the second prime in a strong prime 5‑tuple. A strong
 *            prime 5‑tuple consists of five consecutive primes p0<p1<p2<p3<p4
 *            such that the gaps are strictly decreasing: p1−p0 > p2−p1 >
 *            p3−p2 > p4−p3. This routine lazily builds a sieve of primes,
 *            scans for the pattern and caches the found terms....
 * Range:     u8 none, u16 n <= 171, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* static state ------------------------------------------------------------- */
static uint32_t prime_limit   = 2000000;    /* start sieve limit                   */
static uint32_t *prime_buf    = NULL;      /* array of primes                    */
static size_t   prime_cap    = 0;          /* allocated capacity                  */
static size_t   prime_cnt    = 0;          /* number of primes stored             */

static seq_t *term_buf  = NULL;           /* cached sequence terms              */
static size_t term_cap  = 0;              /* allocated capacity                  */
static size_t term_cnt  = 0;              /* number of terms stored              */

static int initialized = 0;                /* whether state is initialised       */

/* ------------------------------------------------------------------------ */
/*  Grow a dynamic array of uint32_t                                      */
static void ensure_prime_cap(size_t fit)
{
    if (prime_cap >= fit) return;
    size_t new_cap = prime_cap ? prime_cap * 2 : 16;
    while (new_cap < fit) new_cap *= 2;
    uint32_t *tmp = (uint32_t *)realloc(prime_buf, new_cap * sizeof(uint32_t));
    if (!tmp) exit(EXIT_FAILURE);          /* out of memory */
    prime_buf = tmp;
    prime_cap = new_cap;
}

static void ensure_term_cap(size_t fit)
{
    if (term_cap >= fit) return;
    size_t new_cap = term_cap ? term_cap * 2 : 16;
    while (new_cap < fit) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(term_buf, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    term_buf = tmp;
    term_cap = new_cap;
}

/* ------------------------------------------------------------------------ */
/*  Build primes up to 'limit' by sieve (limit <= 2^32-1).                  */
static void build_sieve(uint32_t limit)
{
    /* allocate is_prime flag array */
    uint8_t *is_prime = (uint8_t *)calloc((size_t)limit + 1, 1);
    if (!is_prime) exit(EXIT_FAILURE);
    is_prime[0] = is_prime[1] = 0;
    for (uint32_t i = 2; i <= limit; ++i) is_prime[i] = 1;

    /* quick integer sqrt */
    uint32_t sq = 0;
    while ((sq + 1U) * (sq + 1U) <= limit) ++sq;

    for (uint32_t p = 2; p <= sq; ++p)
        if (is_prime[p])
            for (uint32_t q = p * p; q <= limit; q += p)
                is_prime[q] = 0;

    /* collect primes */
    ensure_prime_cap(limit / 10 + 100);
    prime_cnt = 0;
    for (uint32_t i = 2; i <= limit; ++i)
        if (is_prime[i]) {
            prime_buf[prime_cnt++] = i;
        }

    free(is_prime);
}

/* ------------------------------------------------------------------------ */
/*  Scan the current prime list for decreasing four‑gap patterns            */
static void collect_terms(void)
{
    ensure_term_cap(prime_cnt / 10 + 100);
    for (size_t i = 0; i + 4 < prime_cnt; ++i) {
        seq_t d1 = (seq_t)(prime_buf[i + 1] - prime_buf[i]);
        seq_t d2 = (seq_t)(prime_buf[i + 2] - prime_buf[i + 1]);
        seq_t d3 = (seq_t)(prime_buf[i + 3] - prime_buf[i + 2]);
        seq_t d4 = (seq_t)(prime_buf[i + 4] - prime_buf[i + 3]);
        if (d1 > d2 && d2 > d3 && d3 > d4)
            term_buf[term_cnt++] = (seq_t)prime_buf[i + 1];
    }
}

/* ------------------------------------------------------------------------ */
/*  Public sequence routine                                                 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* safety – offset is 1 */

    if (!initialized) {
        build_sieve(prime_limit);
        collect_terms();
        initialized = 1;
    }

    /* enlarge sieve until we have at least n terms */
    while (term_cnt < (size_t)n) {
        /* grow the sieve by a fixed block (e.g. +5 000 000) */
        prime_limit += 5000000;
        build_sieve(prime_limit);
        collect_terms();
    }

    return term_buf[(size_t)n - 1];
}
