/*
 * A141702  a(n) is the number of Carmichael numbers of the form
 *          prime(n)*prime(n')*prime(n") with n > n' > n".
 * https://oeis.org/A141702
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes OEIS sequence A141702: a(n) is the number of
   Carmichael numbers of the form p*q*r where p = prime(n) is the largest
   prime factor, with n > n' > n''.  The Carmichael condition is
   equivalently
        p-1 | pqr-1, q-1 | pqr-1, r-1 | pqr-1.
   The code uses a simple search over the prime indices for each p, storing
   all primes r<p in a residue-linked list to fast‑select candidates
   satisfying the congruence q*r ≡ 1 (mod p-1).  All other congruences are
   then checked directly.  The algorithm runs in O(n^2 / log n) time and
   uses negligible memory for the problem size (n≤10000). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "../utils/sieve_first_n_primes.h"

/* Static cache of primes */
static uint32_t *primes = NULL;   /* primes[0] = 2, primes[1] = 3, ... */
static size_t   prime_count = 0;  /* number of cached primes */

/* Ensure that the global prime array contains at least `need` primes. */
static void
ensure_primes(size_t need)
{
    if (prime_count >= need)
        return;
    uint32_t *tmp;
    size_t tmp_cnt;
    sieve_first_n_primes((idx_t)need, &tmp, &tmp_cnt);
    free(primes);
    primes = tmp;
    prime_count = tmp_cnt;
}

/* Greatest common divisor for uint32_t. */
static uint32_t
gcd_uint32(uint32_t a, uint32_t b)
{
    while (b) {
        uint32_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Modular inverse of a modulo m (gcd(a,m)=1).  Returns 0 if none. */
static uint32_t
modinv_uint32(uint32_t a, uint32_t m)
{
    int64_t t0 = 0, t1 = 1;
    int64_t r0 = m, r1 = a % m;
    while (r1 != 0) {
        int64_t q = r0 / r1;
        int64_t tmp = t0;
        t0 = t1;
        t1 = tmp - q * t1;
        tmp = r0;
        r0 = r1;
        r1 = tmp - q * r1;
    }
    if (r0 != 1)
        return 0;                       /* no inverse exists */
    if (t0 < 0)
        t0 += m;
    return (uint32_t)t0;
}

/* Main sequence function. */
seq_t
a(idx_t n)
{
    if (n < 3)                /* need at least three primes for a triple */
        return (seq_t)0;

    /* Make sure the prime cache contains the first n primes. */
    ensure_primes((size_t)n);

    /* Index and value of prime(n). */
    uint32_t pPrime = primes[(size_t)n - 1];
    uint32_t pMinus = pPrime - 1;              /* p-1  */
    size_t   pIdx0  = (size_t)n - 1;           /* 0‑based index of p */

    /* Build residue-linked list for all primes < pPrime. */
    int *head = (int *)malloc(pMinus * sizeof(int));
    int *next = (int *)malloc(pIdx0 * sizeof(int));  /* indices 0..pIdx0-1  */
    if (!head || !next)
        return (seq_t)0;                      /* allocation failure */

    for (int i = 0; i < (int)pMinus; ++i)
        head[i] = -1;

    for (int ridx = 1; ridx < (int)pIdx0; ++ridx) {
        uint32_t rPrime = primes[ridx];
        int rem = (int)(rPrime % pMinus);
        next[ridx] = head[rem];
        head[rem] = ridx;
    }

    seq_t count = 0;

    /* Iterate over all possible q < p.  q must also be > r, so qIdx ≥ 2. */
    for (int qIdx = (int)pIdx0 - 1; qIdx >= 2; --qIdx) {
        uint32_t qPrime = primes[qIdx];
        if (gcd_uint32(qPrime, pMinus) != 1)
            continue;                        /* no inverse, skip */

        uint32_t inv_q = modinv_uint32(qPrime, pMinus);
        if (inv_q == 0)
            continue;
        int targetRem = (int)(inv_q % pMinus);

        /* Traverse all r's with residue targetRem. */
        for (int ridx = head[targetRem]; ridx != -1; ridx = next[ridx]) {
            if (ridx >= qIdx)                 /* need r < q */
                continue;
            uint32_t rPrime = primes[ridx];

            /* Check remaining two congruences:  p*r ≡ 1 (mod q-1) */
            uint32_t mod_qm = qPrime - 1;
            uint32_t mod_rm = rPrime - 1;
            if ((uint64_t)pPrime * rPrime % mod_qm != 1)
                continue;
            if ((uint64_t)pPrime * qPrime % mod_rm != 1)
                continue;

            /* All conditions satisfied */
            ++count;
        }
    }

    free(head);
    free(next);
    return count;
}
