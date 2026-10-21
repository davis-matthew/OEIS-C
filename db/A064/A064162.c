/*
 * A064162  Least abundant number divisible by n.
 * https://oeis.org/A064162
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 42, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search‑based implementation: for each n find the smallest
 * multiple of n that is an abundant number (σ(m) > 2m).  */
/* Ref: A064162 – Least abundant number divisible by n. */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* ---------- Prime sieve ---------- */
#define PRIME_LIMIT 1000000          /* ample for n up to 10 000 */
static bool sieve_done = false;
static int prime_cnt = 0;
static int primes[PRIME_LIMIT];

static void init_sieve(void)
{
    if (sieve_done) return;
    static bool marked[PRIME_LIMIT + 1];
    memset(marked, 0, sizeof(marked));
    for (int i = 2; i * i <= PRIME_LIMIT; ++i)
        if (!marked[i])
            for (int j = i * i; j <= PRIME_LIMIT; j += i)
                marked[j] = true;
    for (int i = 2; i <= PRIME_LIMIT; ++i)
        if (!marked[i])
            primes[prime_cnt++] = i;
    sieve_done = true;
}

/* ---------- Sum of divisors σ(n) for 64‑bit input ---------- */
static seq_t sigma_uint64(uint64_t n)
{
    uint64_t res = 1;
    uint64_t t = n;
    for (int i = 0; i < prime_cnt; ++i) {
        int p = primes[i];
        if ((uint64_t)p * (uint64_t)p > t) break;
        if (t % p == 0) {
            uint64_t term = 1, pow = 1;
            do {
                t /= p;
                pow *= p;
                term += pow;
            } while (t % p == 0);
            res *= term;
        }
    }
    if (t > 1) res *= (1 + t);   /* remaining prime factor */
    return (seq_t)res;
}

/* ---------- Check if n is abundant ---------- */
static bool is_abundant(uint64_t n)
{
    uint64_t sig = 1;
    uint64_t t = n;
    for (int i = 0; i < prime_cnt; ++i) {
        int p = primes[i];
        if ((uint64_t)p * (uint64_t)p > t) break;
        if (t % p == 0) {
            uint64_t term = 1, pow = 1;
            do {
                t /= p;
                pow *= p;
                term += pow;
            } while (t % p == 0);
            sig *= term;
            if (sig > 2 * n) return true;  /* early exit */
        }
    }
    if (t > 1) {
        sig *= (1 + t);
        if (sig > 2 * n) return true;
    }
    return sig > 2 * n;
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n)
{
    init_sieve();                     /* ensure primes are ready */

    if (n == 0) return 0;             /* undefined for n=0 */

    uint64_t step = (uint64_t)n;      /* multiple step */
    uint64_t m = step;                /* current candidate     */
    while (true) {
        if (is_abundant(m))
            return (seq_t)m;
        m += step;                    /* next multiple of n   */
    }
}
