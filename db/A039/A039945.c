/*
 * A039945  Maris-McGwire numbers(2): numbers k such that f(k) = f(k+1), where
 *          f(k) = sum of digits of k + sum of digits of the distinct prime
 *          factors of k.
 * https://oeis.org/A039945
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 2970, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes all k such that f(k)=f(k+1) by brute‑force checking of
   consecutive integers. */
/* OEIS A039945: Maris‑McGwire numbers(2) */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* ------------------------------------------------------------------ */
/* Utility: sum of decimal digits of an unsigned 64‑bit integer         */
/* ------------------------------------------------------------------ */
static seq_t sum_digits_uint64(uint64_t n)
{
    unsigned int s = 0;
    while (n) {
        s += n % 10;
        n /= 10;
    }
    return (seq_t)s;
}

/* ------------------------------------------------------------------ */
/* Prime sieve: dynamic array of primes, filled incrementally          */
/* ------------------------------------------------------------------ */
static uint64_t *prime_list = NULL;   /* array of prime numbers          */
static size_t prime_cnt  = 0;         /* number of primes stored         */
static size_t prime_cap  = 0;         /* current capacity of prime_list  */

static void grow_prime_cap(size_t need)
{
    if (prime_cap >= need)
        return;
    size_t new_cap = prime_cap ? prime_cap << 1 : 16;
    while (new_cap < need)
        new_cap <<= 1;
    uint64_t *tmp = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
    if (!tmp)
        exit(EXIT_FAILURE);
    prime_list = tmp;
    prime_cap  = new_cap;
}

static void add_prime(uint64_t p)
{
    grow_prime_cap(prime_cnt + 1);
    prime_list[prime_cnt++] = p;
}

/* Ensure that prime_list contains all primes ≤ limit (inclusive). */
static void ensure_primes_up_to(uint64_t limit)
{
    if (prime_cnt && prime_list[prime_cnt - 1] >= limit)
        return;
    uint64_t candidate = prime_cnt ? prime_list[prime_cnt - 1] + 1ULL : 2ULL;
    while (candidate <= limit) {
        bool is_prime = true;
        for (size_t i = 0; i < prime_cnt && prime_list[i] * prime_list[i] <= candidate; ++i) {
            if (candidate % prime_list[i] == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime)
            add_prime(candidate);
        ++candidate;
    }
}

/* Integer square root (floor) for uint64_t, no math.h. */
static uint64_t int_sqrt_uint64(uint64_t x)
{
    uint64_t r = 0;
    while ((r + 1ULL) * (r + 1ULL) <= x)
        ++r;
    return r;
}

/* ------------------------------------------------------------------ */
/* Compute f(k) = sum_digits(k) + sum_digits(distinct prime factors) */
/* ------------------------------------------------------------------ */
static seq_t f(uint64_t k)
{
    seq_t sum = sum_digits_uint64(k);          /* digits of k          */
    uint64_t n  = k;
    uint64_t limit = int_sqrt_uint64(k);
    ensure_primes_up_to(limit);
    for (size_t i = 0; i < prime_cnt && prime_list[i] <= limit; ++i) {
        uint64_t p = prime_list[i];
        if (n % p == 0) {
            sum += sum_digits_uint64(p);           /* distinct prime factor */
            while (n % p == 0)                     /* remove all copies     */
                n /= p;
        }
    }
    if (n > 1)                                       /* remaining prime       */
        sum += sum_digits_uint64(n);
    return sum;
}

/* ------------------------------------------------------------------ */
/* Dynamic array of found sequence values (k values).                 */
/* ------------------------------------------------------------------ */
static seq_t *seq = NULL;
static size_t seq_len = 0;
static size_t seq_cap = 0;

static void grow_seq_cap(size_t need)
{
    if (seq_cap >= need)
        return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 16;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!tmp)
        exit(EXIT_FAILURE);
    seq = tmp;
    seq_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/* Return the n-th element (1‑based) of the sequence.                 */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;           /* guard for accidental 0‑based callers */

    /* Lazy initialisation of state. */
    static uint64_t last_checked_k = 0;   /* last k evaluated           */
    static seq_t last_f_value = 0;        /* f(last_checked_k) value    */

    if (seq_len == 0) {
        last_checked_k = 1;
        last_f_value   = f(1);
    }

    /* Extend sequence until we have at least n elements. */
    while (seq_len < (size_t)n) {
        ++last_checked_k;                     /* current k to examine   */
        seq_t cur_f = f(last_checked_k);      /* f(k)                   */

        if (cur_f == last_f_value) {
            /* k-1 satisfies f(k-1)=f(k)                 */
            grow_seq_cap(seq_len + 1);
            seq[seq_len++] = (seq_t)(last_checked_k - 1);
        }
        last_f_value = cur_f;
    }

    return seq[n - 1];
}
