/*
 * A082477  Number of divisors d of n such that d+1 is also a divisor of n+1.
 * https://oeis.org/A082477
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # { d | n : (d+1) | (n+1) }
 * Range:     u8 n <= 9204, u16 n <= 9204, u32 n <= 9204, u64 n <= 9204,
 *            u128 n <= 9204, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The routine computes the number of divisors d of n such that
    d+1 also divides n+1.  It uses a small prime sieve to factor n
    and then enumerates all divisors recursively.  This requires
    only O(#prime‑factors + #divisors) work per call.  */
#include <stdbool.h>
#include <stddef.h>

/*------------------------------------------------------------
 *  Prime sieve (once per program run)
 *------------------------------------------------------------*/
#define PRIME_LIMIT 1000000  /* enough for n up to at least 10^12 */
static int  primes[78498];   /* 78498 primes <= 1,000,000 */
static size_t primeCnt = 0;
static bool primesInited = false;

static void init_primes(void)
{
    if (primesInited) return;

    bool sieve[PRIME_LIMIT + 1];
    for (int i = 2; i <= PRIME_LIMIT; ++i) sieve[i] = true;
    for (int i = 2; i * i <= PRIME_LIMIT; ++i)
        if (sieve[i])
            for (int j = i * i; j <= PRIME_LIMIT; j += i)
                sieve[j] = false;

    for (int i = 2; i <= PRIME_LIMIT; ++i)
        if (sieve[i]) primes[primeCnt++] = i;
    primesInited = true;
}

/*------------------------------------------------------------
 *  Recursive enumeration of divisors
 *------------------------------------------------------------*/
static void rec_div(const idx_t *factors, const int *exps, size_t pos,
                    size_t cnt, idx_t cur, idx_t nplus1, seq_t *acc)
{
    if (pos == cnt) {
        if ((nplus1 % (cur + 1ULL)) == 0ULL)
            (*acc)++;
        return;
    }

    idx_t p = factors[pos];
    idx_t v = 1;
    for (int e = 0; e <= exps[pos]; ++e) {
        rec_div(factors, exps, pos + 1, cnt, cur * v, nplus1, acc);
        v *= p;
    }
}

/*------------------------------------------------------------
 *  Main sequence routine
 *------------------------------------------------------------*/
seq_t a(idx_t n)
{
    /* The offset of OEIS is 1; n>=1 is guaranteed by the call interface. */
    init_primes();

    /* Factor n into prime powers. */
    idx_t rem = n;
    idx_t factors[64];
    int   exps   [64];
    size_t cnt = 0;

    for (size_t i = 0; i < primeCnt; ++i) {
        idx_t p = (idx_t)primes[i];
        if (p * p > rem) break;
        if (rem % p == 0) {
            int e = 0;
            do { ++e; rem /= p; } while (rem % p == 0);
            factors[cnt] = p;
            exps[cnt]    = e;
            ++cnt;
        }
    }
    if (rem > 1) {
        factors[cnt] = rem;
        exps[cnt]    = 1;
        ++cnt;
    }

    /* Enumerate all divisors and count the valid ones. */
    seq_t acc = 0;
    rec_div(factors, exps, 0, cnt, 1, n + 1, &acc);
    return acc;
}
