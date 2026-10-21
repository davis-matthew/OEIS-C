/*
 * A067777  Numbers k such that (2*k)! * B(6*k) is an integer where B(k) are
 *          the Bernoulli numbers B(1)=-1/2 B(2)=1/6 B(4)=-1/30... B(2*m+1)=0
 *          m>1.
 * https://oeis.org/A067777
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A067777: numbers k such that (2*k)! · B(6*k) is an integer.
   Equivalent condition: for every prime p with p‑1 | 6k, we have p ≤ 2k. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>

/* Helper data structures for prime generation */
static uint64_t *prime_list = NULL;
static size_t prime_cnt   = 0;
static size_t prime_cap   = 0;
static uint64_t prime_limit = 1;              /* highest number checked for primes */

/* Ensure that all primes up to 'limit' are generated.  The simple
   incremental sieve uses trial division by already-found primes. */
static void ensure_primes(uint64_t limit)
{
    if (limit <= prime_limit) return;

    for (uint64_t num = prime_limit + 1; num <= limit; ++num) {
        bool is_prime = true;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = prime_list[i];
            if (p * p > num) break;
            if (num % p == 0) { is_prime = false; break; }
        }
        if (is_prime) {
            if (prime_cnt == prime_cap) {
                prime_cap = prime_cap ? prime_cap * 2 : 128;
                prime_list = (uint64_t *)realloc(prime_list,
                                                prime_cap * sizeof(uint64_t));
            }
            prime_list[prime_cnt++] = num;
        }
    }
    prime_limit = limit;
}

/* Binary search to test primality of a number that we know is ≤ prime_limit. */
static bool is_prime_num(uint64_t n)
{
    if (n < 2) return false;
    ensure_primes(n);                      /* guarantees primes up to n are known */
    size_t left = 0, right = prime_cnt;
    while (left < right) {
        size_t mid = (left + right) >> 1;
        uint64_t p = prime_list[mid];
        if (p == n) return true;
        if (p < n)  left = mid + 1;
        else        right = mid;
    }
    return false;
}

/* Test whether k satisfies the condition that every prime p with p‑1 | 6k
   obeys p ≤ 2k.  We use the divisor collection of 6k. */
static bool is_good_k(uint64_t k)
{
    uint64_t M = 6 * k;
    uint64_t limit_sqrt = (uint64_t)sqrt((double)M) + 1;

    for (uint64_t d = 1; d <= limit_sqrt; ++d) {
        if (M % d != 0) continue;

        /* First divisor d */
        uint64_t p = d + 1;
        if (p > 2 * k && is_prime_num(p)) return false;

        /* Complementary divisor d2 = M / d */
        uint64_t d2 = M / d;
        if (d2 != d) {
            p = d2 + 1;
            if (p > 2 * k && is_prime_num(p)) return false;
        }
    }
    return true;
}

/* Storage for the generated sequence and state for incremental computation. */
static seq_t *sequence = NULL;
static size_t seq_cnt   = 0;
static size_t seq_cap   = 0;
static uint64_t next_k  = 1;                /* smallest k not yet tested */

/* Public sequence function requested by the build. */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n=0, but never used */

    /* Generate enough terms to satisfy the request.  The generation
       is lazy and memoized; subsequent calls may jump to previously
       computed values. */
    while (seq_cnt < (size_t)n) {
        if (is_good_k(next_k)) {
            seq_t val = (seq_t)next_k;
            if (seq_cnt == seq_cap) {
                seq_cap = seq_cap ? seq_cap * 2 : 128;
                sequence = (seq_t *)realloc(sequence,
                                            seq_cap * sizeof(seq_t));
            }
            sequence[seq_cnt++] = val;
        }
        ++next_k;
    }
    return sequence[n - 1];
}
