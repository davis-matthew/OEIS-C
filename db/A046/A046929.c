/*
 * A046929  Width of moat of composite numbers surrounding n-th prime.
 * https://oeis.org/A046929
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS sequence A046929: Width of moat of composite numbers surrounding the
 * n‑th prime.
 *
 * Formula: a(n) = min( p_n - p_{n-1} , p_{n+1} - p_n ) - 1
 * where p_k is the k‑th prime (p_0 is defined as 0; for n = 1 the
 * left side is considered infinite, so a(1) = 0).
 *
 * The implementation keeps a cached list of primes and extends their size
 * on demand.  For a given n it ensures that primes[0 .. n] (i.e. p_1 .. p_{n+1})
 * are available, then computes the two gaps and returns the smaller one.
 * The sieve uses a classic Eratosthenes algorithm; the upper limit for the
 * sieve is estimated using n*(log n + log log n)+3 (valid for n≥6).
 *
 * The code is valid for C11 and C++17, uses only static linking with
 * standard library functions, and respects the constraints on types.
 */
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

/* --- Prime cache -------------------------------------------------------- */

static uint64_t *g_primes = NULL;     /* array of found primes (0‑based) */
static size_t   g_prime_cnt = 0;      /* number of primes stored */

static size_t estimate_limit(size_t n)
{
    /* For n >= 6:  p_n < n(ln n + ln ln n)
       Add a small safety margin. */
    if (n < 6) return 15; /* primes up to 13 are more than enough */
    double dn = (double)n;
    double lim = dn * (log(dn) + log(log(dn))) + 3.0;
    return (size_t)lim;
}

static void generate_primes(size_t limit, uint64_t **out, size_t *cnt)
{
    /* Simple Eratosthenes sieve up to 'limit' */
    char *is_prime = (char*)malloc(limit + 1);
    if (!is_prime) { *out = NULL; *cnt = 0; return; }

    for (size_t i = 2; i <= limit; ++i) is_prime[i] = 1;

    size_t root = (size_t)sqrt((double)limit);
    for (size_t i = 2; i <= root; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= limit; j += i)
                is_prime[j] = 0;
        }
    }

    /* Count primes */
    *cnt = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (is_prime[i]) ++(*cnt);

    /* Allocate and fill primes array */
    *out = (uint64_t*)malloc(*cnt * sizeof(uint64_t));
    size_t idx = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (is_prime[i]) (*out)[idx++] = (uint64_t)i;

    free(is_prime);
}

static void ensure_primes(idx_t n)
{
    /* Ensure that primes[0 .. n] (i.e. at least n+1 primes) exist. */
    size_t needed = (size_t)n + 1;
    if (g_prime_cnt >= needed) return;

    size_t limit = estimate_limit(needed);
    while (1) {
        uint64_t *tmp = NULL;
        size_t tmp_cnt = 0;
        generate_primes(limit, &tmp, &tmp_cnt);
        if (tmp_cnt >= needed) {
            free(g_primes);
            g_primes = tmp;
            g_prime_cnt = tmp_cnt;
            return;
        }
        free(tmp);
        limit *= 2;   /* try a larger bound if not enough primes */
    }
}

/* --- Sequence function ----------------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n<1, safe fallback */

    /* Make sure we have p_n and p_{n+1}. */
    ensure_primes(n);

    uint64_t p_n = g_primes[(size_t)n - 1];   /* p_n      (1‑based) */
    uint64_t p_next = g_primes[(size_t)n];    /* p_{n+1}  (0‑based) */

    seq_t left  = 0;
    seq_t right = (seq_t)(p_next - p_n - 1);

    if (n > 1) {
        uint64_t p_prev = g_primes[(size_t)n - 2]; /* p_{n-1} */
        left = (seq_t)(p_n - p_prev - 1);
    }

    return (left < right) ? left : right;
}
