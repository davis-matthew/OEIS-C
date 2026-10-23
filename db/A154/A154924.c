/*
 * A154924  Area of prime triangles.
 * https://oeis.org/A154924
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = area of the triangle with vertices (p_n ,p_{n+1}),
 *            (p_{n+2},p_{n+3}), (p_{n+4},p_{n+5}) where p_k is the k‑th prime
 *            (p_1 = 2). The area is computed via the shoelace formula: area =
 *            |x1*(y2–y3) + x2*(y3–y1) + x3*(y1–y2)| / 2
 * Range:     u8 n <= 186, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t *prime_list = NULL;   /* array of generated primes          */
static size_t     prime_cnt   = 0;     /* number of primes currently stored  */
static size_t     prime_alloc = 0;     /* allocated size                     */
static uint64_t   next_candidate = 2;  /* next number to test for primality */

/* Ensure that the array contains at least `needed` primes.  */
static void
ensure_primes(size_t needed)
{
    if (prime_cnt >= needed) return;

    if (prime_cnt + (needed - prime_cnt) > prime_alloc) {
        size_t new_alloc = prime_alloc ? prime_alloc * 2 : 16;
        while (new_alloc < needed) new_alloc <<= 1;
        uint64_t *tmp = (uint64_t*)realloc(prime_list,
                                          new_alloc * sizeof(uint64_t));
        if (!tmp) abort();
        prime_list = tmp;
        prime_alloc = new_alloc;
    }

    while (prime_cnt < needed) {
        uint64_t candidate = next_candidate;
        bool is_prime = true;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = prime_list[i];
            if (p * p > candidate) break;
            if (candidate % p == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) prime_list[prime_cnt++] = candidate;

        /* Advance to the next candidate (skip even numbers). */
        if (candidate == 2) next_candidate = 3;
        else                 next_candidate += 2;
    }
}

/* The sequence entry a(n).  n is 1‑based. */
seq_t
a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n < 1 */

    /* We need the primes p_n … p_{n+5} → total n+5 primes. */
    size_t needed = (size_t)n + 5;
    ensure_primes(needed);

    size_t i = (size_t)(n - 1);           /* 0‑based index of p_n           */
    uint64_t x1 = prime_list[i];
    uint64_t y1 = prime_list[i + 1];
    uint64_t x2 = prime_list[i + 2];
    uint64_t y2 = prime_list[i + 3];
    uint64_t x3 = prime_list[i + 4];
    uint64_t y3 = prime_list[i + 5];

    /* Shoelace cross product (twice the signed area). */
    int64_t cross =
        (int64_t)x1 * (int64_t)(y2 - y3) +
        (int64_t)x2 * (int64_t)(y3 - y1) +
        (int64_t)x3 * (int64_t)(y1 - y2);

    uint64_t abs_cross = cross >= 0 ? (uint64_t)cross
                                   : (uint64_t)(-cross);

    seq_t area = (seq_t)(abs_cross / 2ULL);
    return area;
}
