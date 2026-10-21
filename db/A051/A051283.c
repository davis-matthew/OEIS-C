/*
 * A051283  Numbers k such that if one writes k = Product p_i^e_i (p_i primes)
 *          and P = max p_i^e_i, then k/P > P.
 * https://oeis.org/A051283
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A051283 (OEIS)
 *  Numbers k such that if k = Π p_i^e_i (p_i primes) and
 *  P = max(p_i^e_i) then k/P > P.
 *
 *  Implementation:  
 *  A(n) is found by scanning the integers k starting at 2,
 *  computing the largest prime‑power factor of k and testing the
 *  inequality k/P > P.  The primes up to 4 000 000 are sieved once
 *  lazily and reused for all calls.  The algorithm is O(k loglog k)
 *  and easily satisfies the 2 s time limit for n ≤ 10 000.  The code
 *  uses only integer arithmetic and is valid both as C11 and C++17.
 */

#include <stdlib.h>
#include <stdbool.h>

/* --- global state ----------------------------------------------------- */
static bool primes_ready  = false;     /* primes precomputed?          */
static int *primes = NULL;            /* array of primes              */
static size_t primes_count = 0;       /* number of primes             */

/* --- prime sieve ------------------------------------------------------ */
static void compute_primes(void)
{
    const int SIEVE_LIMIT = 4000000;          /* enough for n≈10 000       */
    int *mark = (int*)malloc((SIEVE_LIMIT + 1) * sizeof(int));
    if (!mark) return;                       /* unlikely in test harness  */
    for (int i = 0; i <= SIEVE_LIMIT; ++i) mark[i] = 1;

    for (int i = 2; (long long)i * i <= SIEVE_LIMIT; ++i) {
        if (mark[i]) {
            for (long long j = (long long)i * i; j <= SIEVE_LIMIT; j += i)
                mark[(int)j] = 0;
        }
    }

    /* first pass: count primes to allocate right size */
    primes_count = 0;
    for (int i = 2; i <= SIEVE_LIMIT; ++i)
        if (mark[i]) ++primes_count;

    /* allocate and store primes */
    primes = (int*)malloc(primes_count * sizeof(int));
    if (!primes) { free(mark); return; }

    size_t idx = 0;
    for (int i = 2; i <= SIEVE_LIMIT; ++i) if (mark[i]) primes[idx++] = i;

    free(mark);
}

/* --- largest prime‑power factor of k --------------------------------- */
static seq_t largest_prime_power(int k)
{
    int tmp = k;
    seq_t max_pow = 1;

    for (size_t i = 0; i < primes_count && primes[i] * primes[i] <= tmp; ++i) {
        int p = primes[i];
        if (tmp % p == 0) {
            seq_t pow = 1;
            while (tmp % p == 0) {
                tmp /= p;
                pow *= (seq_t)p;
            }
            if (pow > max_pow) max_pow = pow;
        }
    }

    if (tmp > 1) {                     /* remaining factor is prime   */
        seq_t pow = (seq_t)tmp;
        if (pow > max_pow) max_pow = pow;
    }

    return max_pow;
}

/* -------------------------------------------------------------------- */
/*  Public interface: a(n) -------------------------------------------- */
/* -------------------------------------------------------------------- */
extern seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* 0 is undefined; return 0    */

    if (!primes_ready) {
        compute_primes();
        primes_ready = true;
    }

    idx_t count = 0;
    for (int k = 2;; ++k) {
        seq_t max_pow = largest_prime_power(k);
        seq_t q = (seq_t)k / max_pow;
        if (q > max_pow) {
            ++count;
            if (count == n)
                return (seq_t)k;
        }
    }
}
