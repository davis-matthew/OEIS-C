/*
 * A036267  6th differences of primes.
 * https://oeis.org/A036267
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = p_{n+6} - 6 p_{n+5} + 15 p_{n+4} - 20 p_{n+3} + 15
 *            p_{n+2} - 6 p_{n+1} + p_n (forward 6th difference of primes)
 * Range:     i8 n <= 94, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static uint64_t *prime_buf = NULL;   /* array of primes (0‑based) */
static size_t prime_cnt = 0;         /* number of primes stored */
static size_t prime_cap = 0;         /* allocated capacity */
static uint64_t cand_start = 2;      /* next integer to test for primality */

/* ensure prime_buf holds at least m primes (m is 1‑based count) */
static void ensure_primes(size_t m)
{
    if (prime_cnt >= m) return;
    /* increase capacity if needed */
    if (prime_cnt + (m - prime_cnt) > prime_cap) {
        size_t new_cap = (prime_cap == 0) ? (m * 2) : (prime_cap * 2);
        if (new_cap < m) new_cap = m;
        uint64_t *tmp = (uint64_t *)realloc(prime_buf, new_cap * sizeof(uint64_t));
        if (!tmp) abort();                   /* allocation failure */
        prime_buf = tmp;
        prime_cap = new_cap;
    }

    /* simple trial‑division prime generator */
    for (uint64_t cand = cand_start; prime_cnt < m; ++cand, ++cand_start) {
        int is_prime = 1;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = prime_buf[i];
            if (p * p > cand) break;
            if (cand % p == 0) { is_prime = 0; break; }
        }
        if (is_prime) {
            prime_buf[prime_cnt++] = cand;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for offset <1, but safe return */
    idx_t needed = n + 6;   /* need primes up to p_{n+6} */
    ensure_primes((size_t)needed);

    /* fetch the required primes (1‑based indexing) */
    seq_t p0 = (seq_t)prime_buf[(size_t)(n - 1 + 0)]; /* p_n   */
    seq_t p1 = (seq_t)prime_buf[(size_t)(n - 1 + 1)]; /* p_{n+1} */
    seq_t p2 = (seq_t)prime_buf[(size_t)(n - 1 + 2)]; /* p_{n+2} */
    seq_t p3 = (seq_t)prime_buf[(size_t)(n - 1 + 3)]; /* p_{n+3} */
    seq_t p4 = (seq_t)prime_buf[(size_t)(n - 1 + 4)]; /* p_{n+4} */
    seq_t p5 = (seq_t)prime_buf[(size_t)(n - 1 + 5)]; /* p_{n+5} */
    seq_t p6 = (seq_t)prime_buf[(size_t)(n - 1 + 6)]; /* p_{n+6} */

    /* compute the 6th forward difference */
    seq_t result = p6 - 6 * p5 + 15 * p4 - 20 * p3 + 15 * p2 - 6 * p1 + p0;
    return result;
}
