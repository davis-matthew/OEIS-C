/*
 * A105155  A triangular array made from a negative 6-fold permutation
 *          product.
 * https://oeis.org/A105155
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, uned
 * Formula:   * Let r be the smallest integer such that r(r+1)/2 >= n. * Let c
 *            = n - (r-1)r/2 (so 1 <= c <= r). * Let p_k denote the k-th prime
 *            (p_1 = 2). * Define the row multiplier m(r) by: * m(1) = p_3 =
 *            5, * m(2) = p_2 = 3, * m(3) = p_1 = 2, * m(r) = p_r for r >= 4.
 *            * Then a(n) = p_c * m(r)
 * Range:     u8 n <= 27, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

static seq_t primes[200000];
static idx_t prime_count;

/* Simple sieve to pre‑compute primes up to a fixed limit. */
static void init_primes(void)
{
    static bool done = false;
    if (done) return;
    const idx_t LIMIT = 200000;
    bool sieve[LIMIT + 1];
    for (idx_t i = 0; i <= LIMIT; ++i) sieve[i] = true;
    sieve[0] = sieve[1] = false;
    for (idx_t i = 2; (idx_t)i * (idx_t)i <= LIMIT; ++i)
        if (sieve[i])
            for (idx_t j = i * i; j <= LIMIT; j += i)
                sieve[j] = false;
    prime_count = 0;
    for (idx_t i = 2; i <= LIMIT; ++i)
        if (sieve[i]) primes[prime_count++] = i;
    done = true;
}

static idx_t find_row(idx_t n)
{
    /* Binary search for the row r: r(r+1)/2 >= n */
    idx_t lo = 1, hi = n;
    while (lo < hi) {
        idx_t mid = (lo + hi) >> 1;
        unsigned __int128 t = (unsigned __int128)mid * (mid + 1) / 2;
        if (t >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;           /* undefined for n<1 but guard anyway */
    init_primes();

    idx_t r = find_row(n);                     /* row index */
    unsigned __int128 prev_sum = (unsigned __int128)(r - 1) * r / 2;
    idx_t c = n - (idx_t)prev_sum;              /* column index, 1 <= c <= r */

    seq_t prime_c = primes[c - 1];              /* p_c */
    seq_t multiplier;
    if (r == 1)
        multiplier = primes[2];                 /* p_3 */
    else if (r == 2)
        multiplier = primes[1];                 /* p_2 */
    else if (r == 3)
        multiplier = primes[0];                 /* p_1 */
    else
        multiplier = primes[r - 1];             /* p_r for r >= 4 */

    return prime_c * multiplier;
}
