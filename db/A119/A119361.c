/*
 * A119361  a(n) is the n-th positive integer which is divisible by the same
 *          distinct primes as n and which is divisible by no other primes.
 * https://oeis.org/A119361
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) is the n-th positive integer that is divisible by the same
 * distinct primes as n and by no others.  For a given n this is the
 * n-th term of the sequence of numbers of the form rad(n)·p₁ˢ¹…p_kˢᵏ
 * with each exponent s_i ≥ 0 (rad(n) is the product of the distinct
 * primes dividing n).
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

/* local helper: factorize n and collect its distinct prime factors.
 * n is assumed to be a uint64_t, and primes[] must be large enough to
 * hold all distinct primes of n.  cnt is set to the number of primes found. */
static void factor_uint64(uint64_t n, uint64_t primes[], size_t *cnt)
{
    *cnt = 0;
    /* trial division up to sqrt(n) */
    for (uint64_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            primes[(*cnt)++] = p;
            while (n % p == 0) n /= p;
        }
    }
    if (n > 1) primes[(*cnt)++] = n;
}

/* The requested sequence function.  seq_t is an unsigned integer type
 * (8, 16, 32, 64, 128 bits or an arbitrary‑precision type).  Index n
 * is 1‑based. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, but guard */

    /* Step 1: factor n to obtain its distinct primes. */
    uint64_t primes[64];
    size_t prime_cnt;
    factor_uint64((uint64_t)n, primes, &prime_cnt);

    /* Step 2: compute rad(n) = product of distinct primes. */
    seq_t rad = 1;
    for (size_t i = 0; i < prime_cnt; ++i) {
        rad = rad * (seq_t)primes[i];
    }

    /* Step 3: generate the Hamming‑type sequence for this set of primes.
     * We generate the first n terms in ascending order and return a[n-1]. */
    seq_t *arr = (seq_t *)malloc(sizeof(seq_t) * (size_t)n);
    if (!arr) return 0;  /* allocation failure */

    arr[0] = rad;
    size_t idx[64] = {0};

    for (size_t i = 1; i < (size_t)n; ++i) {
        /* compute the next candidate values for each prime. */
        seq_t cand[64];
        for (size_t j = 0; j < prime_cnt; ++j) {
            cand[j] = arr[idx[j]] * (seq_t)primes[j];
        }
        /* find the minimum among candidates. */
        seq_t min_val = cand[0];
        for (size_t j = 1; j < prime_cnt; ++j) {
            if (cand[j] < min_val) min_val = cand[j];
        }
        arr[i] = min_val;
        /* advance pointers for primes that produced the minimum. */
        for (size_t j = 0; j < prime_cnt; ++j) {
            if (cand[j] == min_val) idx[j] += 1;
        }
    }

    seq_t result = arr[(size_t)n - 1];
    free(arr);
    return result;
}
