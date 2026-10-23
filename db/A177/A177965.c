/*
 * A177965  Indices m for which A177961(m) - m = 1.
 * https://oeis.org/A177965
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (A002476(n-1)+1)/2 with a(1)=1. A002476(n) for n≥1 is the
 *            sequence of primes ≡1 (mod 6) with an initial value
 *            A002476(0)=1. Thus for n≥2 we find the (n-1)‑st prime ≡1 (mod 6)
 *            and return (p+1)/2
 * Range:     u8 n <= 46, u16 n <= 6104, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Dynamic arrays for all primes and for primes ≡1 (mod 6). */
static uint64_t *primes_all  = NULL;
static size_t   primes_all_cnt = 0;
static size_t   primes_all_cap = 0;

static uint64_t *primes_mod6  = NULL;
static size_t   primes_mod6_cnt = 0;
static size_t   primes_mod6_cap = 0;

/* Next odd candidate to test for primality (starts at 5). */
static uint64_t next_candidate = 5;

/* Grow a dynamic uint64_t array. */
static void grow_array(uint64_t **arr, size_t *cnt, size_t *cap)
{
    if (*cnt < *cap)
        return;
    size_t new_cap = (*cap == 0) ? 32 : (*cap << 1);
    uint64_t *tmp = (uint64_t *)realloc(*arr, new_cap * sizeof(uint64_t));
    if (!tmp)
        abort();
    *arr = tmp;
    *cap = new_cap;
}

/* Append a prime to the all‑primes table. */
static void add_prime_all(uint64_t p)
{
    grow_array(&primes_all, &primes_all_cnt, &primes_all_cap);
    primes_all[primes_all_cnt++] = p;
}

/* Append a prime to the 1‑mod‑6 table. */
static void add_prime_mod6(uint64_t p)
{
    grow_array(&primes_mod6, &primes_mod6_cnt, &primes_mod6_cap);
    primes_mod6[primes_mod6_cnt++] = p;
}

/* Test primality of n using the list of known primes (all primes < n). */
static int is_prime_uint64(uint64_t n)
{
    if (n < 2)
        return 0;
    for (size_t i = 0; i < primes_all_cnt; ++i) {
        uint64_t p = primes_all[i];
        if (p > n / p)   /* p*p > n, use division to avoid overflow */
            break;
        if (n % p == 0)
            return 0;
    }
    return 1;
}

/* Ensure the list of primes ≡1 (mod 6) contains at least (target+1)
   elements.  target is a zero‑based index. */
static void ensure_primes_mod6(idx_t target)
{
    if (primes_mod6_cnt > (size_t)target)
        return;

    /* Initialize the primes list with 2 and 3 if it hasn't been started. */
    if (primes_all_cnt == 0) {
        add_prime_all(2);
        add_prime_all(3);
    }

    while (primes_mod6_cnt <= (size_t)target) {
        uint64_t cand = next_candidate;
        if (is_prime_uint64(cand)) {
            add_prime_all(cand);
            if ((cand % 6) == 1)
                add_prime_mod6(cand);
        }
        next_candidate = cand + 2;   /* advance to the next odd number */
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    idx_t target = n - 2;             /* zero‑based index into primes_mod6 */
    ensure_primes_mod6(target);

    uint64_t p  = primes_mod6[(size_t)target];
    seq_t   res = (seq_t)((p + 1) / 2);
    return res;
}
