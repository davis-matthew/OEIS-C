/*
 * A054495  Smallest k such that n/k is a Fibonacci number.
 * https://oeis.org/A054495
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the smallest divisor k of n such that n/k is a Fibonacci
 *            number
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* ---------- global data for primes and Fibonacci numbers ---------- */
#define MAX_PRIME 1000000U

static uint32_t prime_list[MAX_PRIME + 1];
static int prime_count = 0;
static uint64_t fibs[100];
static int fib_count = 0;

/* ---------- utility: sieve for primes up to MAX_PRIME ---------- */
static void init_primes(void)
{
    static int done = 0;
    if (done) return;
    int isprime[MAX_PRIME + 1];
    for (int i = 0; i <= MAX_PRIME; ++i) isprime[i] = 1;
    isprime[0] = isprime[1] = 0;
    for (int i = 2; i * i <= MAX_PRIME; ++i) {
        if (isprime[i]) {
            for (int j = i * i; j <= MAX_PRIME; j += i) isprime[j] = 0;
        }
    }
    prime_count = 0;
    for (int i = 2; i <= MAX_PRIME; ++i)
        if (isprime[i]) prime_list[prime_count++] = (uint32_t)i;
    done = 1;
}

/* ---------- utility: generate Fibonacci numbers up to UINT64_MAX ---------- */
static void init_fibs(void)
{
    static int done = 0;
    if (done) return;
    fibs[0] = 1;
    fibs[1] = 1;
    fib_count = 2;
    while (1) {
        uint64_t a = fibs[fib_count - 2];
        uint64_t b = fibs[fib_count - 1];
        uint64_t next = a + b;
        if (next < b) break;                /* overflow detected */
        if (next > 0xFFFFFFFFFFFFFFFFULL) break;
        fibs[fib_count++] = next;
    }
    done = 1;
}

/* ---------- check if a number is Fibonacci (binary search) ---------- */
static bool is_fib(uint64_t x)
{
    int lo = 0, hi = fib_count - 1;
    while (lo <= hi) {
        int mid = (lo + hi) >> 1;
        if (fibs[mid] == x) return true;
        if (fibs[mid] < x) lo = mid + 1;
        else hi = mid - 1;
    }
    return false;
}

/* ---------- data structure for prime factors ---------- */
typedef struct {
    uint64_t p;
    int      e;
} Factor;

/* ---------- recursive divisor generation ---------- */
static void gen_divs(int idx,
                     uint64_t cur,
                     const Factor *fac,
                     int facCnt,
                     uint64_t n,
                     uint64_t *best)
{
    if (idx == facCnt) {
        uint64_t m = n / cur;
        if (is_fib(m) && cur < *best) *best = cur;
        return;
    }
    uint64_t p = fac[idx].p;
    int e = fac[idx].e;
    uint64_t val = 1;
    for (int i = 0; i <= e; ++i) {
        gen_divs(idx + 1, cur * val, fac, facCnt, n, best);
        val *= p;
    }
}

/* ---------- the sequence function ---------- */
seq_t a(idx_t n)
{
    /* initialise helpers once */
    init_primes();
    init_fibs();

    /* factor n into primes */
    uint64_t m = n;
    Factor fac[64];
    int facCnt = 0;

    for (int i = 0; i < prime_count && (uint64_t)prime_list[i] * (uint64_t)prime_list[i] <= m; ++i) {
        uint64_t p = prime_list[i];
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            fac[facCnt].p = p;
            fac[facCnt].e = e;
            ++facCnt;
        }
    }
    if (m > 1) {                       /* remaining prime factor */
        fac[facCnt].p = m;
        fac[facCnt].e = 1;
        ++facCnt;
    }

    /* search all divisors for the smallest k */
    uint64_t best = n;                 /* k can never exceed n */
    gen_divs(0, 1, fac, facCnt, n, &best);

    return (seq_t)best;
}
