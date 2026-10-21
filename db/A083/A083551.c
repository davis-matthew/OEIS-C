/*
 * A083551  Least common multiple of 2 consecutive prime differences, of two
 *          successive terms of A001223.
 * https://oeis.org/A083551
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lcm(A001223(n), A001223(n+1)) where A001223(k) =
 *            prime(k+1) – prime(k) (prime(1)=2, prime(2)=3, …)
 * Range:     u8 n <= 1863, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* ----------------------------------------------------------------
   Prime table (1‑based indexing):
   prime_list[0] = 2, prime_list[1] = 3, …
   ---------------------------------------------------------------- */
static idx_t *prime_list = NULL;   /* array of primes */
static size_t prime_count = 0;     /* number of primes stored */
static idx_t sieve_limit = 0;      /* largest number that has been sieved */

/* ----------------------------------------------------------------
   Euclidean GCD for seq_t values.
   Works for unsigned integer or arbitrary‑precision integer types.
   ---------------------------------------------------------------- */
static seq_t seq_gcd(seq_t a, seq_t b)
{
    while (b != 0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* ----------------------------------------------------------------
   Simple sieve of Eratosthenes.
   Computes all primes ≤ limit and stores them in prime_list.
   --------------------------------------------------------------- */
static void compute_primes_upto(idx_t limit)
{
    bool *is_comp = (bool *)calloc((size_t)(limit + 1), sizeof(bool));
    if (!is_comp) exit(EXIT_FAILURE);

    /* Rough estimate for π(limit) to reserve enough space. */
    size_t est_cnt = (limit > 20) ?
        (size_t)(limit / (log((double)limit) - 1.5)) + 10 : 50;
    if (est_cnt < 10) est_cnt = 10;

    free(prime_list);
    prime_list = (idx_t *)malloc(est_cnt * sizeof(idx_t));
    if (!prime_list) { free(is_comp); exit(EXIT_FAILURE); }
    size_t cnt = 0;

    for (idx_t i = 2; i <= limit; ++i) {
        if (!is_comp[i]) {
            if (cnt >= est_cnt) {
                est_cnt *= 2;
                prime_list = (idx_t *)realloc(prime_list, est_cnt * sizeof(idx_t));
                if (!prime_list) { free(is_comp); exit(EXIT_FAILURE); }
            }
            prime_list[cnt++] = i;

            if ((uint64_t)i * i <= limit) {
                for (idx_t j = i * i; j <= limit; j += i)
                    is_comp[j] = true;
            }
        }
    }
    prime_count = cnt;
    sieve_limit = limit;
    free(is_comp);
}

/* ----------------------------------------------------------------
   Ensure that at least `need` primes are stored.
   `need` is the 1‑based number of primes required (≥ 1).
   --------------------------------------------------------------- */
static void ensure_primes(idx_t need)
{
    if (prime_count >= need) return;

    /* Rosser–Schoenfeld bound: p_n < n (ln n + ln ln n) for n ≥ 6. */
    double n = (double)need;
    idx_t limit;
    if (need < 6)
        limit = 15;
    else
        limit = (idx_t)(n * (log(n) + log(log(n))) + 3.0);

    /* Increase the limit until enough primes are generated. */
    while (prime_count < need) {
        if (limit <= sieve_limit) limit = sieve_limit * 2;
        compute_primes_upto(limit);
        if (prime_count >= need) break;
        limit *= 2;
    }
}

/* ----------------------------------------------------------------
   Public sequence function.
   ---------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;  /* Undefined for n < 1, but safe to return 0. */

    /* We need primes up to index n+2 (i.e. p_{n+2}). */
    ensure_primes(n + 2);

    /* A001223(k) = prime(k+1) – prime(k) with 1‑based primes. */
    seq_t gap1 = (seq_t)prime_list[n]     - (seq_t)prime_list[n - 1];
    seq_t gap2 = (seq_t)prime_list[n + 1] - (seq_t)prime_list[n];

    seq_t g = seq_gcd(gap1, gap2);
    seq_t lcm = (gap1 / g) * gap2;  /* lcm(gap1, gap2) */

    return lcm;
}
