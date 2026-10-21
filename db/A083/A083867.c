/*
 * A083867  a(n) is the number of divisors of the n-th decimal palindrome that
 *          are palindromes.
 * https://oeis.org/A083867
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   is_pal_digit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = number of palindromic divisors of the n‑th decimal palindrome. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_pal_digit.h"

/* ------------------------------------------------------------------ */
/* Small prime sieve (up to 50000) – used for trial division.  The     */
/* array is filled on first use.                                      */
/* ------------------------------------------------------------------ */
static uint32_t g_primes[5200];
static int g_prime_cnt = 0;

static void init_primes(void)
{
    if (g_prime_cnt)
        return;
    const uint32_t LIMIT = 50000U;
    bool is_prime[LIMIT + 1];
    for (unsigned i = 0; i <= LIMIT; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (uint32_t i = 2; i * i <= LIMIT; ++i)
        if (is_prime[i])
            for (uint32_t j = i * i; j <= LIMIT; j += i)
                is_prime[j] = false;
    for (uint32_t i = 2; i <= LIMIT; ++i)
        if (is_prime[i]) g_primes[g_prime_cnt++] = i;
}

/* ------------------------------------------------------------------ */
/* Computes 10^e for small e (fits in uint64_t).                       */
/* ------------------------------------------------------------------ */
static uint64_t pow10(uint32_t e)
{
    uint64_t r = 1ULL;
    while (e--) r *= 10ULL;
    return r;
}

/* ------------------------------------------------------------------ */
/* Check whether a 64‑bit integer is a decimal palindrome.            */

/* ------------------------------------------------------------------ */
/* Return the nth (1‑based) decimal palindrome as a uint64_t.           */
/* ------------------------------------------------------------------ */
static uint64_t nth_palindrome(idx_t n)
{
    /* Find length L such that cumulative count >= n. */
    uint64_t total = 0;
    uint32_t L = 0;
    while (1) {
        ++L;
        uint32_t half = (L + 1) / 2;
        uint64_t count_len = 9ULL * ((half == 1) ? 1ULL : pow10(half - 1));
        if (total + count_len >= n) break;
        total += count_len;
    }
    uint32_t half = (L + 1) / 2;
    uint64_t rank = n - total;            /* rank within this length (1‑based) */
    uint64_t first_half = pow10(half - 1) + (rank - 1);

    /* Build full palindrome by mirroring. */
    uint64_t res = first_half;
    uint64_t rev = first_half;
    if (L & 1) rev /= 10;                 /* skip middle digit for odd length */
    while (rev > 0) {
        res = res * 10ULL + (rev % 10ULL);
        rev /= 10ULL;
    }
    return res;
}

/* ------------------------------------------------------------------ */
/* Recursive divisor enumeration.                                     */
/* factors array holds distinct prime factors and their exponents.   */
/* ------------------------------------------------------------------ */
struct Factor { uint64_t p; int e; };

static void count_pal_divisors(const struct Factor *fac, int faccnt,
                               int idx, uint64_t cur, seq_t *cnt)
{
    if (idx == faccnt) {
        if (is_pal_digit(cur)) (*cnt)++;
        return;
    }
    uint64_t p = fac[idx].p;
    int exp = fac[idx].e;
    uint64_t v = cur;
    for (int k = 0; k <= exp; ++k) {
        count_pal_divisors(fac, faccnt, idx + 1, v, cnt);
        v *= p;
    }
}

/* ------------------------------------------------------------------ */
/* Main sequence function                                               */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* offset is 1; n==0 is outside. */
    init_primes();

    uint64_t pal = nth_palindrome(n);

    /* Factor pal using trial division. */
    struct Factor fac[20];
    int faccnt = 0;
    uint64_t temp = pal;
    for (int i = 0; i < g_prime_cnt && (uint64_t)g_primes[i] * g_primes[i] <= temp; ++i) {
        uint32_t p = g_primes[i];
        if (temp % p) continue;
        int e = 0;
        while (temp % p == 0) {
            temp /= p;
            ++e;
        }
        fac[faccnt].p = (uint64_t)p;
        fac[faccnt].e = e;
        ++faccnt;
    }
    if (temp > 1) {                     /* remaining prime factor > sqrt(original) */
        fac[faccnt].p = temp;
        fac[faccnt].e = 1;
        ++faccnt;
    }

    seq_t count = 0;
    count_pal_divisors(fac, faccnt, 0, 1ULL, &count);
    return count;
}
