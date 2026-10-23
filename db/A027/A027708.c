/*
 * A027708  n-th prime has odd number of 1's in binary expansion and (n+1)st
 *          prime also has odd number of 1's.
 * https://oeis.org/A027708
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 81, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function returns the 1‑based index k of the k‑th prime in a
   pair of consecutive primes whose binary expansion contains an odd
   number of 1 bits.  a(0) = 4.  For n ≥ 0, a(n) is the n‑th such index. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* popcount for uint64_t */
static inline uint64_t popcount64(uint64_t x)
{
#if defined(__GNUC__) || defined(__clang__) || defined(_MSC_VER)
    return __builtin_popcountll((unsigned long long)x);
#else
    uint64_t c = 0;
    while (x)
    {
        c += x & 1;
        x >>= 1;
    }
    return c;
#endif
}

/* dynamic arrays for primes and matching indices */
static uint64_t *prime_list = NULL;
static size_t prime_len   = 0;
static size_t prime_cap   = 0;

static seq_t *match_idx = NULL;
static size_t match_len = 0;
static size_t match_cap = 0;

/* next odd candidate to test for primality */
static uint64_t next_candidate = 3;

/* initialise prime list with 2 */
static void init_primes(void)
{
    if (prime_list == NULL)
    {
        prime_cap   = 16;
        prime_list  = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
        prime_list[0] = 2;
        prime_len   = 1;
    }
}

/* ensure the match array has enough capacity */
static void ensure_match_cap(size_t need)
{
    while (match_cap < need)
    {
        size_t new_cap = match_cap ? match_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(match_idx, new_cap * sizeof(seq_t));
        match_idx = tmp;
        match_cap = new_cap;
    }
}

/* trial division test using already found primes */
static bool is_prime(uint64_t cand)
{
    for (size_t i = 0; i < prime_len; ++i)
    {
        uint64_t p = prime_list[i];
        if (p > cand / p)      /* stop when p*p > cand, without overflow */
            break;
        if (cand % p == 0)
            return false;
    }
    return true;
}

/* generate primes until we have at least (n+1) matching indices */
static void generate_n_indices(idx_t n)
{
    init_primes();
    ensure_match_cap((size_t)n + 1);

    while (match_len <= (size_t)n)
    {
        /* find next prime */
        uint64_t cand = next_candidate;
        while (!is_prime(cand))
            cand += 2;

        /* add prime to list */
        if (prime_len == prime_cap)
        {
            size_t new_cap = prime_cap * 2;
            uint64_t *tmp = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
            prime_list = tmp;
            prime_cap  = new_cap;
        }
        prime_list[prime_len++] = cand;
        next_candidate = cand + 2;   /* skip even numbers */

        /* check pair with previous prime if possible */
        if (prime_len >= 2)
        {
            uint64_t prev = prime_list[prime_len - 2];
            uint64_t cur  = prime_list[prime_len - 1];

            if (((popcount64(prev) & 1U) != 0) &&
                ((popcount64(cur)  & 1U) != 0))
            {
                /* store 1‑based index of previous prime */
                seq_t val = (seq_t)(prime_len - 1);
                match_idx[match_len++] = val;
            }
        }
    }
}

/* public function required by the build system */
seq_t a(idx_t n)
{
    generate_n_indices(n);
    return match_idx[(size_t)n];
}
