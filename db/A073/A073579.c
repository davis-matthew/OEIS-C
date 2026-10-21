/*
 * A073579  Signed primes: if prime(n) even, a(n) = 0; if prime(n) == 1 (mod
 *          4), a(n) = prime(n); if prime(n) == -1 (mod 4), a(n) = -prime(n).
 * https://oeis.org/A073579
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(1) = 0; for n>1, let p = nth prime. Then a(n) = 0 if p == 2, p
 *            if p % 4 == 1, -p if p % 4 == 3. Example: a(6) = 13, a(8) = -19
 * Range:     i8 n <= 31, i16 n <= 3512, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Trivial prime generator using trial division and incremental growth. */

static uint64_t *prime_list = NULL;   /* list of primes found so far */
static size_t prime_cnt   = 0;         /* number of primes stored */
static size_t prime_cap   = 0;         /* allocated capacity */

/* Expand the prime list if needed. */
static void ensure_prime_capacity(size_t needed)
{
    if (prime_cap >= needed) return;
    size_t new_cap = prime_cap ? prime_cap * 2 : 16;
    if (new_cap < needed) new_cap = needed;
    uint64_t *new_list = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
    if (!new_list) abort(); /* allocation failure */
    prime_list = new_list;
    prime_cap = new_cap;
}

/* Generate primes until we have at least 'n' primes (1‑based). */
static void generate_primes_up_to(size_t n)
{
    if (prime_cnt >= n) return;
    /* start searching from the next odd number after the last prime */
    uint64_t cand = (prime_cnt == 0) ? 2 : prime_list[prime_cnt - 1] + 1;
    if (cand % 2 == 0 && cand != 2) cand++; /* skip even numbers */

    while (prime_cnt < n) {
        int is_prime = 1;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = prime_list[i];
            if (p * p > cand) break;
            if (cand % p == 0) { is_prime = 0; break; }
        }
        if (is_prime) {
            ensure_prime_capacity(prime_cnt + 1);
            prime_list[prime_cnt++] = cand;
        }
        cand += 2;
        if (cand == 2) cand = 3; /* after handling 2, skip to odd */
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset is 1, but we defensively handle 0 */
    generate_primes_up_to((size_t)n);
    uint64_t p = prime_list[n - 1];  /* nth prime (1‑based) */
    if (p == 2) return 0;
    if (p % 4 == 1) return (seq_t)p;
    return -(seq_t)p;
}
