/*
 * A071822  Numbers whose largest prime factor is of the form 4k+3.
 * https://oeis.org/A071822
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 140, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth integer whose largest prime factor ≡ 3 (mod 4)
   (offset 1).  The implementation builds the sequence lazily,
   storing known terms and primes in static arrays. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ---------- prime storage ---------- */
static uint32_t *prime_list   = NULL;
static size_t   prime_cnt    = 0;
static size_t   prime_cap    = 0;

/* add a new prime to the list */
static void add_prime(uint32_t p)
{
    if (prime_cnt == prime_cap) {
        prime_cap = prime_cap ? prime_cap << 1 : 8;
        prime_list = (uint32_t *)realloc(prime_list, prime_cap * sizeof(uint32_t));
    }
    prime_list[prime_cnt++] = p;
}

/* fill prime_list with all primes ≤ limit (uses simple trial division). */
static void ensure_primes_up_to(uint32_t limit)
{
    if (prime_cnt == 0) {          /* initialise with 2 and 3 */
        add_prime(2);
        add_prime(3);
    }
    if (limit <= prime_list[prime_cnt - 1])
        return;                     /* already have sufficient primes */

    /* start searching from the next odd after the last known prime */
    uint32_t start = prime_list[prime_cnt - 1] + 1;
    if ((start & 1) == 0)          /* make it odd */
        ++start;

    for (uint32_t n = start; n <= limit; n += 2) {
        bool is_prime = true;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint32_t p = prime_list[i];
            if ((uint64_t)p * p > n)
                break;
            if (n % p == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime)
            add_prime(n);
    }
}

/* integer square‑root (floor) for 64‑bit values. */
static uint32_t int_sqrt(uint64_t x)
{
    uint32_t r = 0;
    while ((uint64_t)(r + 1) * (r + 1) <= x)
        ++r;
    return r;
}

/* largest prime factor of n (returns 1 if n ≤ 1). */
static uint64_t largest_prime_factor(uint64_t n)
{
    if (n <= 1)
        return 1;
    uint32_t limit = int_sqrt(n);
    ensure_primes_up_to(limit);

    uint64_t x = n;
    uint64_t lpf = 1;

    for (size_t i = 0; i < prime_cnt && (uint64_t)prime_list[i] * prime_list[i] <= x; ++i) {
        uint32_t p = prime_list[i];
        if (x % p == 0) {
            lpf = p;
            while (x % p == 0)
                x /= p;
        }
    }
    if (x > 1)                /* remaining prime factor > sqrt(original n) */
        lpf = x;
    return lpf;
}

/* ---------- sequence storage ---------- */
static seq_t *seq_terms = NULL;
static size_t seq_len   = 0;
static size_t seq_cap   = 0;

/* append a term to seq_terms */
static void add_term(seq_t val)
{
    if (seq_len == seq_cap) {
        seq_cap = seq_cap ? seq_cap << 1 : 8;
        seq_terms = (seq_t *)realloc(seq_terms, seq_cap * sizeof(seq_t));
    }
    seq_terms[seq_len++] = val;
}

/* find the next integer after 'start' whose largest prime factor ≡ 3 (mod 4) */
static uint64_t next_candidate(uint64_t start)
{
    uint64_t m = start + 1;
    for (;;) {
        uint64_t lpf = largest_prime_factor(m);
        if (lpf % 4 == 3)
            return m;
        ++m;
    }
}

/* the requested term a(n).  n is 1‑based. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;        /* undefined offset */

    static uint64_t last_checked = 0;   /* largest number already inspected */
    while (seq_len < (size_t)n) {
        uint64_t next = next_candidate(last_checked);
        last_checked = next;
        add_term((seq_t)next);
    }
    return seq_terms[n - 1];
}
