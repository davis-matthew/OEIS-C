/*
 * A024460  a(n) = s(1)t(n) + s(2)t(n-1) + ... + s(k)t(n+1-k), where k = [
 *          (n+1)/2 ], s = (Fibonacci numbers), t = (1, p(1), p(2), ...).
 * https://oeis.org/A024460
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 28, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = Σ_{i=1}^{⌊(n+1)/2⌋} Fib(i) * t(n+1-i)
 * where Fib(1)=1, Fib(2)=1, Fib(k)=Fib(k-1)+Fib(k-2) for k≥3
 * and t(1)=1, t(k)=Prime(k-1) for k>1 (Prime(1)=2, Prime(2)=3, …).
 */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* static caches for Fibonacci numbers and primes */
static seq_t *fib_cache = NULL;      /* fib_cache[i] = Fib(i) */
static size_t fib_cap   = 0;         /* computed up to fib_cap-1, inclusive */

static uint64_t *prime_cache = NULL; /* prime_cache[0] = 2 = Prime(1) */
static size_t   prime_cnt   = 0;     /* number of primes stored */

/* ensure fib_cache contains Fib values up to index 'idx' (inclusive) */
static void ensure_fib(size_t idx)
{
    if (fib_cap > idx) return;

    size_t new_cap = idx + 1;
    seq_t *buf = (seq_t*)realloc(fib_cache, new_cap * sizeof(seq_t));
    if (!buf) exit(EXIT_FAILURE);
    fib_cache = buf;

    if (fib_cap == 0) {
        fib_cache[0] = (seq_t)0;
        if (new_cap > 1) fib_cache[1] = (seq_t)1;
        fib_cap = (new_cap > 1) ? 2 : 1;
    }

    for (size_t i = fib_cap; i <= idx; ++i) {
        fib_cache[i] = fib_cache[i - 1] + fib_cache[i - 2];
    }
    fib_cap = new_cap;
}

/* generate enough primes so that prime_cache contains at least 'need' primes */
/* primes are stored 0‑based: prime_cache[0] = 2 = Prime(1) */
static void ensure_primes(size_t need)
{
    if (prime_cnt >= need) return;

    /* choose an upper bound that is likely to contain 'need' primes */
    size_t limit;
    if (need < 6) limit = 15;
    else {
        double n = (double)need;
        limit = (size_t)(n * (log(n) + log(log(n))) + 10.0);
    }

    bool *is_comp = (bool*)calloc(limit + 1, sizeof(bool));
    if (!is_comp) exit(EXIT_FAILURE);

    /* allocate memory for primes */
    uint64_t *buf = (prime_cnt == 0)
        ? (uint64_t*)malloc(need * sizeof(uint64_t))
        : (uint64_t*)realloc(prime_cache, need * sizeof(uint64_t));
    if (!buf) exit(EXIT_FAILURE);
    prime_cache = buf;

    size_t count = 0;
    for (size_t p = 2; p <= limit && count < need; ++p) {
        if (!is_comp[p]) {
            prime_cache[count++] = (uint64_t)p;
            if (p * p <= limit) {
                for (size_t q = p * p; q <= limit; q += p)
                    is_comp[q] = true;
            }
        }
    }

    free(is_comp);
    prime_cnt = count;
    /* count should be ≥ need; if not, we would need a larger limit,
       but the chosen estimate guarantees this for need ≤ 10000
       (useful for the required n range). */
}

/* t(k) as defined in the sequence: 1 for k==1, otherwise prime(k-1) */
static seq_t t_val(idx_t k)
{
    if (k == 1) return (seq_t)1;
    return (seq_t)prime_cache[k - 2];  /* prime_cache[0] = Prime(1) = 2 */
}

/* required sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* out of range guard */

    /* compute k = floor((n+1)/2) */
    size_t k = (size_t)((n + 1) >> 1);    /* integer division by 2 */
    ensure_fib(k);
    if (n > 1) ensure_primes((size_t)n - 1);

    seq_t sum = (seq_t)0;
    for (size_t i = 1; i <= k; ++i) {
        seq_t s = fib_cache[i];
        idx_t t_index = n + 1 - (idx_t)i;
        seq_t t = t_val(t_index);
        sum += s * t;
    }
    return sum;
}
