/*
 * A084695  Triangle read by rows in which row n lists the n smallest positive
 *          numbers k such that k + n is a prime.
 * https://oeis.org/A084695
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = prime(π(r) + m) – r where r = smallest integer with
 *            r(r+1)/2 ≥ n, m = n – r(r−1)/2 – 1 (0‑based offset in row r)
 * Range:     u8 n <= 1175, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* ----------  Prime sieve (on‑demand)  --------------------------------- */
static struct {
    seq_t *primes;   /* array of primes  (1‑based values) */
    size_t count;    /* number of primes stored     */
    size_t cap;      /* allocated capacity          */
    size_t limit;    /* largest number tested so far */
} primes_state = {NULL, 0, 0, 0};

/* Sieve a new block of numbers up to new_limit, inclusive. */
static void add_primes_up_to(size_t new_limit)
{
    if (new_limit <= primes_state.limit) return;          /* nothing to do */

    /* Ensure storage */
    if (!primes_state.primes) {
        primes_state.cap    = 64;
        primes_state.primes = (seq_t *)malloc(primes_state.cap * sizeof(seq_t));
    }

    /* Start from the first number that has not been checked yet */
    size_t start = primes_state.limit + 1;
    if (start < 2) start = 2;

    for (size_t cand = start; cand <= new_limit; ++cand) {
        int isprime = 1;
        for (size_t i = 0; i < primes_state.count; ++i) {
            seq_t p = primes_state.primes[i];
            if (p * p > cand) break;
            if (cand % p == 0) { isprime = 0; break; }
        }
        if (isprime) {
            if (primes_state.count == primes_state.cap) {
                primes_state.cap <<= 1;
                primes_state.primes = (seq_t *)realloc(
                        primes_state.primes, primes_state.cap * sizeof(seq_t));
            }
            primes_state.primes[primes_state.count++] = (seq_t)cand;
        }
    }
    primes_state.limit = new_limit;
}

/* Ensure that at least “needed” primes have been generated. */
static void ensure_primes_upto_index(size_t needed)
{
    if (primes_state.count >= needed) return;
    size_t limit = primes_state.limit ? primes_state.limit * 2 : 100;
    while (primes_state.count < needed) {
        add_primes_up_to(limit);
        limit <<= 1;
    }
}

/* Count primes ≤ r (π(r)).  Binary search on the primes array. */
static size_t prime_pi(size_t r)
{
    size_t lo = 0, hi = primes_state.count;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (primes_state.primes[mid] <= r) lo = mid + 1;
        else                               hi = mid;
    }
    return lo;  /* number of primes ≤ r */
}

/* ----------  Main routine  -------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n < 1, but required to return something */

    /* Find row r such that r(r+1)/2 ≥ n */
    idx_t r = 1;
    while (r * (r + 1) / 2 < n) ++r;

    /* Offset within the row (0‑based) */
    idx_t prev = r * (r - 1) / 2;
    idx_t m    = n - prev - 1;      /* 0 ≤ m < r */

    /* Ensure the prime list covers r and at least the needed index */
    add_primes_up_to((size_t)r);                /* primes up to r for π(r) */
    size_t pi_r = prime_pi((size_t)r);
    idx_t prime_index = (idx_t)pi_r + m + 1;    /* 1‑based index into primes array */

    ensure_primes_upto_index((size_t)prime_index);   /* guarantee that many primes exist */

    seq_t prime = primes_state.primes[prime_index - 1]; /* 0‑based array */
    return prime - r;
}
