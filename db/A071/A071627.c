/*
 * A071627  The n-th term of Chernoff sequence A006939 divided by (n + 1)!.
 * https://oeis.org/A071627
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = A006939(n) / (n+1)! where A006939(n) = 2 * ∏_{k=2}^{n}
 *            (p1 * p2 * … * pk) (p1 = 2, p2 = 3, … are the primes in order)
 *            Equivalently, for the j‑th prime pj (1‑based) the exponent in
 *            A006939(n) is expA(pj) = n for j = 1 n - j + 1 for j ≥ 2 Then
 *            a(n) = ∏_{j=1}^{n} pj^{expA(pj) – expF(pj)}, whe...
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 7, u128 n <= 10,
 *            big any n
 * Helpers:   legendre_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/legendre_idx.h"

#ifndef MAX_PRIME_LIMIT
#  define MAX_PRIME_LIMIT 200000   /* enough for the 10 000th prime (≈104 729) */
#endif

/* Global storage for primes – computed once on the first call */
static int primes[MAX_PRIME_LIMIT];
static int prime_cnt = 0;
static bool primes_inited = false;

/* Simple sieve to fill the primes array */
static void init_primes(void)
{
    if (primes_inited)
        return;
    primes_inited = true;

    bool is_prime[MAX_PRIME_LIMIT + 1];
    for (int i = 0; i <= MAX_PRIME_LIMIT; ++i)
        is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    for (int i = 2; i * i <= MAX_PRIME_LIMIT; ++i) {
        if (!is_prime[i])
            continue;
        for (int j = i * i; j <= MAX_PRIME_LIMIT; j += i)
            is_prime[j] = false;
    }

    for (int i = 2; i <= MAX_PRIME_LIMIT; ++i)
        if (is_prime[i])
            primes[prime_cnt++] = i;
}

/* Binary exponentiation for seq_t values */
static seq_t pow_seq_t(seq_t base, idx_t exp)
{
    seq_t res = 1;
    while (exp) {
        if (exp & 1u)
            res *= base;
        base *= base;
        exp >>= 1u;
    }
    return res;
}

/* Core function implementing the sequence */
seq_t a(idx_t n)
{
    init_primes();

    seq_t result = 1;

    /* We need primes up to the n-th prime (1‑based index) */
    for (int idx = 0; idx < prime_cnt && idx + 1 <= n; ++idx) {
        idx_t p = primes[idx];              /* prime value */
        idx_t expA = (idx == 0) ? n : n - idx; /* exponent in A006939(n) */
        idx_t expF = legendre_idx(n + 1, p);   /* exponent in (n+1)! */
        idx_t exp = expA > expF ? expA - expF : 0;
        if (exp > 0)
            result *= pow_seq_t((seq_t)p, exp);
    }

    return result;
}
