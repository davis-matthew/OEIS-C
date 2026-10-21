/*
 * A067536  Numbers k such that phi(k*(k+1)*(k+2)/3)/phi(k) is not an integer
 *          where phi(k) is the Euler totient function A000010(k).
 * https://oeis.org/A067536
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 631, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A067536
    a(n) = the n-th integer k ≥ 1 such that
           φ( k·(k+1)·(k+2) / 3 ) is NOT a multiple of φ(k)
    where φ is Euler's totient function.
    The sequence is computed lazily; values are cached in order
    and can be requested in any order. */

#include <stddef.h>
#include <stdlib.h>

/* ---------- configuration ----------
   SIEVE_MAX must be larger than the largest k that is needed to
   generate the requested number of terms.  The default of 2 000 000
   comfortably covers the first 10 000 terms. ----------
*/
#define SIEVE_MAX 2000004   /* must allow k+2 to be indexed */

/* ---------- SPF sieve ----------
   spf[i] is the smallest prime factor of i (for i ≤ SIEVE_MAX).
   The sieve is built once on first use. ----------
*/
static int spf[SIEVE_MAX + 1];
static int spf_ready = 0;

static void build_spf(void)
{
    if (spf_ready) return;
    for (int i = 0; i <= SIEVE_MAX; ++i) spf[i] = i;
    for (int i = 2; i * i <= SIEVE_MAX; ++i)
        if (spf[i] == i)
            for (int j = i * i; j <= SIEVE_MAX; j += i)
                if (spf[j] == j) spf[j] = i;
    spf_ready = 1;
}

/* ---------- small integer power ----------
   Constrained exponent, safe for all seq_t types. ----------
*/
static seq_t pow_seq_t(seq_t base, int exp)
{
    seq_t result = 1;
    while (exp--) result *= base;
    return result;
}

/* ---------- φ(k) for k ≤ SIEVE_MAX ----------
   Factorization using the global SPF array. ----------
*/
static seq_t phi_small(unsigned long long n)
{
    seq_t result = 1;
    while (n > 1) {
        int p = spf[(int)n];
        int exp = 0;
        while (n % p == 0) {
            n /= p;
            ++exp;
        }
        result *= pow_seq_t((seq_t)p, exp - 1) * (seq_t)(p - 1);
    }
    return result;
}

/* ---------- φ for N = k·(k+1)·(k+2)/3 ----------
   Factor the three numbers, combine exponents,
   then remove a single factor of 3. ----------
*/
static seq_t phi_N_from_k(unsigned long long k)
{
    /* local small tables for distinct primes */
    int primes[64];
    int exps[64];
    size_t sz = 0;

    /* helper to add exponent of a prime (merge if already present) */
    for (int part = 0; part < 3; ++part) {
        unsigned long long n = k + part;   /* part = 0 → k, 1 → k+1, 2 → k+2 */
        while (n > 1) {
            int p = spf[(int)n];
            int exp = 0;
            while (n % p == 0) {
                n /= p;
                ++exp;
            }
            /* search for p in primes array */
            size_t i;
            for (i = 0; i < sz; ++i)
                if (primes[i] == p) break;
            if (i == sz) {   /* new prime */
                primes[sz] = p;
                exps[sz] = exp;
                ++sz;
            } else {
                exps[i] += exp;
            }
        }
    }

    /* remove one factor of 3 (division by 3) */
    for (size_t i = 0; i < sz; ++i)
        if (primes[i] == 3) {
            if (exps[i] == 1) {
                /* exponent becomes 0 – drop this prime from list */
                primes[i] = primes[sz - 1];
                exps[i] = exps[sz - 1];
                --sz;
            } else {
                --exps[i];
            }
            break;
        }

    /* compute φ(N) from the combined exponents */
    seq_t result = 1;
    for (size_t i = 0; i < sz; ++i) {
        int p = primes[i];
        int e = exps[i];
        result *= pow_seq_t((seq_t)p, e - 1) * (seq_t)(p - 1);
    }
    return result;
}

/* ---------- cached sequence ----------
   cache holds the k-values that satisfy the condition.
   next_k is the next integer to test. ----------
*/
static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_capacity = 0;
static idx_t next_k = 1;

/* ---------- the required function ----------
   Parameters:
     n – 1-based index (n ≥ 1)
   Returns: the n-th A067536 member. ----------
*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n = 0, harmless */

    build_spf();

    /* extend cache until we have at least n members */
    while (cache_size < (size_t)n) {
        unsigned long long k = next_k++;
        if (k > SIEVE_MAX) {
            /* Too large for our SPF sieve – this code path is
               not expected for typical n (≤ 10 000). */
            break;
        }

        seq_t phi_k = phi_small(k);
        seq_t phi_N = phi_N_from_k(k);

        if (phi_N % phi_k != 0) {
            /* ensure cache has space */
            if (cache_capacity == 0) {
                cache_capacity = 64;
                cache = (seq_t *)malloc(cache_capacity * sizeof(seq_t));
            } else if (cache_size == cache_capacity) {
                cache_capacity <<= 1;
                cache = (seq_t *)realloc(cache, cache_capacity * sizeof(seq_t));
            }
            cache[cache_size++] = (seq_t)k;
        }
    }

    return cache[n - 1];
}
