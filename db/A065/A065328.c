/*
 * A065328  a(n) is the number of primes less than or equal to prime(n) - n.
 * https://oeis.org/A065328
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = π( prime(n) – n ) (idx(t) = 1‑based)
 * Range:     u8 n <= 60, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static uint64_t *prime_list   = NULL;   /* stored primes */
static size_t   prime_cnt    = 0;       /* current number of primes */
static size_t   prime_cap    = 0;       /* capacity of prime_list   */
static uint64_t current_limit = 1;      /* last integer tested for primality */

/* Push a new prime onto the list, growing the array when necessary */
static void push_prime(uint64_t p)
{
    if (prime_cnt == prime_cap) {
        size_t new_cap = prime_cap ? prime_cap << 1 : 16;
        uint64_t *tmp = (uint64_t*)realloc(prime_list, new_cap * sizeof(uint64_t));
        if (!tmp) abort();          /* allocation failure → fatal */
        prime_list = tmp;
        prime_cap  = new_cap;
    }
    prime_list[prime_cnt++] = p;
}

/* Ensure the list contains at least `need` primes (1‑based). */
static void ensure_n_primes(idx_t need)
{
    while (prime_cnt < need) {
        ++current_limit;
        uint64_t x = current_limit;
        if (x < 2) continue;
        int is_prime = 1;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = prime_list[i];
            if (p * p > x) break;
            if (x % p == 0) { is_prime = 0; break; }
        }
        if (is_prime) push_prime(x);
    }
}

/* Count primes ≤ `limit` (limit is a seq_t). */
static size_t prime_count_leq(seq_t limit)
{
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        seq_t p   = (seq_t)prime_list[mid];
        if (p <= limit) lo = mid + 1;
        else            hi = mid;
    }
    return lo;          /* number of primes ≤ limit */
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;      /* undefined for n<1 */
    ensure_n_primes(n);              /* make sure pₙ exists */
    seq_t pn   = (seq_t)prime_list[n - 1];      /* pₙ */
    seq_t lim  = pn - (seq_t)n;                      /* pₙ – n */
    size_t cnt = prime_count_leq(lim);   /* π(lim) */
    return (seq_t)cnt;
}
