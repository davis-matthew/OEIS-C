/*
 * A117503  Primes among partial sums of floor(Pi*prime(k)), k=1,2,3,....
 * https://oeis.org/A117503
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, less
 * Formula:   a(n) = the n-th prime among the partial sums of
 *            floor(π·prime(k))
 * Range:     u8 none, u16 n <= 6, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Validated: partial, pass, 28 terms through n = 28 (2026-11-03)
 * Helpers:   is_prime_u64, mem, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation performed by generating primes, accumulating sums, and
 * collecting those sums that are prime.  Results are memoized so that
 * out‑of‑order or repeated calls incur no additional work. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/next_prime.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* allocate an array of seq_t of given length (may be 0) */
static seq_t *seq_alloc_array(size_t n)
{
    return mem_new(seq_t, n);
}

/* Helper to grow a sequence array of type seq_t. */
static void grow_seq_array(seq_t **arr, size_t *size, size_t new_len)
{
    if (*size >= new_len) return;
    size_t new_cap = (*size > 0) ? (*size << 1) : 16;
    while (new_cap < new_len) new_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, *arr, new_cap);
    if (!tmp) abort();
    *arr = tmp;
    *size = new_cap;
}

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

static const long double PI = 3.14159265358979323846264338327950288L;

seq_t a(idx_t n)
{
    /* Static state for memoisation */
    static uint64_t last_prime = 0;        /* last prime processed */
    static seq_t running_sum = 0;          /* current partial sum     */
    static seq_t *prime_sums = NULL;      /* array of prime sums     */
    static size_t ps_cap = 0;              /* capacity of prime_sums  */
    static size_t ps_len = 0;              /* number of collected sums */

    /* Initialise the array on first call */
    if (ps_cap == 0) {
        ps_cap    = 64;
        prime_sums = seq_alloc_array(ps_cap);
    }

    /* Generate primes and sums until we have at least n primes among sums */
    while (ps_len < n) {
        uint64_t p;
        if (last_prime == 0)
            p = 2;                     /* first prime */
        else
            p = next_prime(last_prime);/* next prime > last_prime */

        last_prime = p;                       /* update for next iteration */

        /* floor(π · p)  (use truncation of positive long double) */
        uint64_t add_rest = (uint64_t)((long double)p * PI);
        seq_t addend = (seq_t)add_rest;

        running_sum += addend;          /* accumulate the sum */

        /* Check if the new sum is prime */
        if (is_prime_big(running_sum)) {
            /* Grow array if necessary */
            if (ps_len >= ps_cap)
                grow_seq_array(&prime_sums, &ps_cap, ps_len + 1);
            prime_sums[ps_len++] = running_sum; /* store the prime sum */
        }
    }

    return prime_sums[n - 1];           /* 1‑based indexing as per OEIS */
}
