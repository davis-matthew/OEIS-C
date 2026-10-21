/*
 * A087876  a[n] =PrimePi[PrimePi[n-2]] + a[n - a[n-2]].
 * https://oeis.org/A087876
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = PrimePi[PrimePi[n-2]] + a[n - a[n-2]] Initial values:
 *            a(1)=1, a(2)=1, a(3)=1, a(4)=1 The sequence is generated
 *            iteratively, storing computed terms so that calls to a() may
 *            come in any order. A simple sieve of Eratosthenes is used to
 *            compute PrimePi up to the required index. All allocations...
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/*  PrimePi: prime_pi[i] = number of primes <= i.
    prime_pi_max records the largest i for which the array is valid.   */
static idx_t prime_pi_max = 0;
static idx_t *prime_pi = NULL;

static void ensure_prime_pi(idx_t nmax)
{
    if (prime_pi_max >= nmax)
        return;

    /* free previous data, if any */
    free(prime_pi);
    prime_pi = (idx_t*)calloc((size_t)(nmax + 1), sizeof(idx_t));
    if (!prime_pi)
        abort();  /* out of memory */

    /* Sieve to find primes up to nmax */
    uint8_t *is_prime = (uint8_t*)calloc((size_t)(nmax + 1), sizeof(uint8_t));
    if (!is_prime)
        abort();

    for (idx_t i = 2; i <= nmax; ++i)
        is_prime[i] = 1;
    for (idx_t i = 2; i * i <= nmax; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= nmax; j += i)
                is_prime[j] = 0;
        }
    }

    /* Build the prefix array of prime counts */
    idx_t count = 0;
    prime_pi[0] = 0;
    prime_pi[1] = 0;
    for (idx_t i = 2; i <= nmax; ++i) {
        if (is_prime[i])
            ++count;
        prime_pi[i] = count;
    }

    free(is_prime);
    prime_pi_max = nmax;
}

/*  Sequence array.  a_arr[i] holds a(i).  a_arr[0] is unused.   */
static seq_t *a_arr = NULL;
static idx_t a_arr_cap = 0;       /* capacity of a_arr, allocated size */
static idx_t a_next_index = 1;    /* next index to compute (1-based) */

static void ensure_a_array(idx_t n)
{
    if (a_arr_cap >= n + 1)
        return;
    size_t new_cap = (a_arr_cap == 0) ? 16 : a_arr_cap * 2;
    while (new_cap < (size_t)(n + 1))
        new_cap <<= 1;
    seq_t *tmp = (seq_t*)realloc(a_arr, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();
    a_arr = tmp;
    a_arr_cap = (idx_t)new_cap;
}

/* Compute all a(i) for i in [a_next_index, n] */
static void compute_a_up_to(idx_t n)
{
    /* Ensure PrimePi array covers the necessary indices */
    if (n >= 2)
        ensure_prime_pi(n - 2);

    ensure_a_array(n);

    for (idx_t i = a_next_index; i <= n; ++i) {
        seq_t val;
        if (i <= 4) {
            val = (seq_t)1;
        } else {
            idx_t idx1 = i - 2;                 /* n-2                         */
            /* Term1 = PrimePi[PrimePi[n-2]]     */
            idx_t pi_idx1 = prime_pi[idx1];      /* inner index   */
            seq_t term1 = (seq_t)prime_pi[pi_idx1];
            /* Term2 = a[n - a[n-2]]             */
            idx_t idx2 = i - (idx_t)a_arr[idx1];
            seq_t term2 = a_arr[idx2];
            val = term1 + term2;
        }
        a_arr[i] = val;
    }
    a_next_index = n + 1;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;             /* undefined for n<1 */
    compute_a_up_to(n);
    return a_arr[n];
}
