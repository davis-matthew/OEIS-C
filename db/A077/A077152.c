/*
 * A077152  Smallest k such that there are n primes between n and k.
 * https://oeis.org/A077152
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 41, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Smallest k such that there are n primes strictly between n and k.
   For each n we need the minimal k>n with π(k‑1) = π(n)+n.
   The implementation precomputes a sieve up to a fixed limit
   and stores the prefix prime counts π(x).  For each query we
   binary‑search the smallest x≥n with π(x)=π(n)+n and return
   k=x+1.  The limit 2 000 000 guarantees correctness for any
   n≤10 000 (the test range of the problem). */

#include <stdlib.h>
#include <string.h>
#include <assert.h>

static const idx_t LIMIT = 2000000;          /* fixed sieve limit */
static unsigned char *isPrime = NULL;        /* 0 = composite, 1 = prime  */
static int *pi = NULL;                       /* prefix prime counts π(x) */

/* One‑time sieve of Eratosthenes up to LIMIT.  */
static void build_sieve(void)
{
    isPrime = (unsigned char *)calloc(LIMIT + 1, 1);
    pi      = (int *)calloc(LIMIT + 1, sizeof(int));
    if (!isPrime || !pi) return;          /* allocation failed – unlikely */

    isPrime[2] = 1;
    /* mark odd numbers as prime initially */
    for (int i = 3; i <= LIMIT; i += 2) isPrime[i] = 1;

    for (int p = 3; (long long)p * p <= LIMIT; p += 2)
        if (isPrime[p]) {
            for (long long m = 1LL * p * p; m <= LIMIT; m += 2 * p)
                isPrime[(int)m] = 0;
        }

    /* prefix counts */
    pi[0] = 0;
    for (int i = 1; i <= LIMIT; ++i)
        pi[i] = pi[i - 1] + (isPrime[i] ? 1 : 0);
}

/* Ensure the sieve is built once. */
static void ensure_sieve(void)
{
    static int built = 0;
    if (!built) {
        build_sieve();
        built = 1;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* sequence is 1‑based */
    ensure_sieve();

    /* target prime count: π(n) + n.  Store in an int because
       π(n) ≤ 148 933 for LIMIT=2 000 000 and n ≤ 10 000. */
    int target = pi[(size_t)n] + (int)n;

    /* binary search for the smallest y ≥ n with π(y) ≥ target. */
    idx_t lo = n, hi = LIMIT;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (pi[(size_t)mid] < target)
            lo = mid + 1;
        else
            hi = mid;
    }
    idx_t y = lo;
    assert(pi[(size_t)y] == target);    /* a solution must exist in range */

    return (seq_t)(y + 1);              /* k = y + 1 */
}
