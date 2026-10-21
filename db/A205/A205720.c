/*
 * A205720  Numbers k for which 10 divides prime(k)-prime(j) for some j<k;
 *          each k occurs once for each such j.
 * https://oeis.org/A205720
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A205720:
   For each prime index k (starting at 1), let r = prime(k) mod 10.
   Append the value k to the sequence exactly as many times as there
   are earlier primes (indices < k) with the same residue r.
   The terms are generated in increasing order of k. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static seq_t *seq_cache = NULL;     /* values of the sequence */
static size_t seq_len  = 0;
static size_t seq_cap  = 0;

static uint64_t *prime_cache = NULL;   /* list of primes found so far */
static size_t prime_len  = 0;
static size_t prime_cap  = 0;

// residue counts (how many primes seen with each digit 0‑9)
static unsigned res_count[10] = {0};

/* --- dynamic array helpers ------------------------------------------------- */
static void grow_seq_cache(size_t need)
{
    if (seq_cap >= need)
        return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 128;
    if (new_cap < need)
        new_cap = need * 2;
    seq_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    seq_cap   = new_cap;
}

static void grow_prime_cache(size_t need)
{
    if (prime_cap >= need)
        return;
    size_t new_cap = prime_cap ? prime_cap * 2 : 32;
    if (new_cap < need)
        new_cap = need * 2;
    prime_cache = (uint64_t *)realloc(prime_cache, new_cap * sizeof(uint64_t));
    prime_cap   = new_cap;
}

/* --- simple primality test using the primes we have found so far ---> */
static bool is_prime_candidate(uint64_t n)
{
    if (n < 2)
        return false;
    if (n == 2 || n == 3)
        return true;
    if ((n & 1) == 0)
        return false;
    for (size_t i = 0; i < prime_len; ++i) {
        uint64_t p = prime_cache[i];
        if (p * p > n)
            break;
        if (n % p == 0)
            return false;
    }
    return true;
}

/* --- ensure seq_cache contains at least `n` terms --------------------- */
static void ensure_seq_size(idx_t n)
{
    if (seq_len >= (size_t)n)
        return;

    while (seq_len < (size_t)n) {
        /* find next prime */
        uint64_t cand = 2;
        if (prime_len > 0)
            cand = prime_cache[prime_len - 1] + 1;
        for (;;) {
            if (is_prime_candidate(cand)) break;
            ++cand;
        }

        /* store prime */
        grow_prime_cache(prime_len + 1);
        prime_cache[prime_len++] = cand;

        /* new prime index (1‑based) */
        size_t k = prime_len;

        /* residue modulo 10 */
        unsigned r = (unsigned)(cand % 10);

        /* how many earlier primes share this residue */
        size_t same = res_count[r];

        /* add k `same` times to the sequence */
        if (same > 0) {
            grow_seq_cache(seq_len + same);
            for (size_t i = 0; i < same; ++i)
                seq_cache[seq_len++] = (seq_t)k;
        }

        /* update residue count */
        res_count[r] = (unsigned)(same + 1);
    }
}

seq_t a(idx_t n)
{
    if (n == 0) /* sequence is defined only for n >= 1 */
        return 0;
    ensure_seq_size(n);
    return seq_cache[n - 1];
}
