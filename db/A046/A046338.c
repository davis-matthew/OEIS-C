/*
 * A046338  Palindromes > 0 with an even number of prime factors (counted with
 *          multiplicity).
 * https://oeis.org/A046338
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 381, u32 n <= 9702, u64 n <= 9702,
 *            u128 n <= 9702, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A046338: Palindromes > 0 with an even number of prime factors (counted with multiplicity) */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Pre‑computed powers of 10 up to 10^19 (fits in 64‑bit). */
static const uint64_t pow10_table[20] = {
    1ULL, 10ULL, 100ULL, 1000ULL, 10000ULL,
    100000ULL, 1000000ULL, 10000000ULL, 100000000ULL, 1000000000ULL,
    10000000000ULL, 100000000000ULL, 1000000000000ULL,
    10000000000000ULL, 100000000000000ULL, 1000000000000000ULL,
    10000000000000000ULL, 100000000000000000ULL, 1000000000000000000ULL
};

/* Simple prime sieve for trial division (sufficient up to sqrt(10^6)). */
static uint64_t primes[80000];
static size_t primes_cnt = 0;
static bool primes_inited = false;

static void init_primes(void)
{
    if (primes_inited) return;
    const uint32_t limit = 1000000U;                  /* > sqrt(max palindrome) */
    unsigned char *composite = (unsigned char *)calloc(limit + 1U, 1U);
    if (!composite) exit(1);
    for (uint32_t i = 2U; i <= limit; ++i) {
        if (!composite[i]) {
            primes[primes_cnt++] = i;
            if ((uint64_t)i * i <= limit) {
                for (uint32_t j = i * i; j <= limit; j += i)
                    composite[j] = 1U;
            }
        }
    }
    free(composite);
    primes_inited = true;
}

/* Count prime factors of x (with multiplicity) using the prime list. */
static unsigned int count_prime_factors(uint64_t x)
{
    init_primes();
    unsigned int cnt = 0;
    for (size_t i = 0; i < primes_cnt && (uint64_t)primes[i] * primes[i] <= x; ++i) {
        uint64_t p = primes[i];
        while (x % p == 0) {
            ++cnt;
            x /= p;
        }
    }
    if (x > 1) ++cnt;          /* remaining prime factor */
    return cnt;
}

/* Construct the palindrome of total length `len` from the left part `left`. */
static uint64_t make_palindrome(uint64_t left, int len)
{
    int half_len = (len + 1) / 2;                /* length of left part */
    uint64_t rev = 0;
    uint64_t tmp = (len & 1) ? left / 10 : left; /* drop middle digit if odd */
    while (tmp > 0) {
        rev = rev * 10ULL + (tmp % 10ULL);
        tmp /= 10ULL;
    }
    uint64_t pow = pow10_table[half_len - (len & 1)];
    return left * pow + rev;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;           /* undefined for n < 1; return 0 */
    idx_t found = 0;
    for (int len = 1; ; ++len) {
        int half = (len + 1) / 2;
        uint64_t left_start = (len == 1) ? 1ULL : pow10_table[half - 1];
        uint64_t left_end   = pow10_table[half] - 1ULL;
        for (uint64_t left = left_start; left <= left_end; ++left) {
            uint64_t pal = make_palindrome(left, len);
            if ((count_prime_factors(pal) & 1) == 0) {   /* even number of factors */
                ++found;
                if (found == n) {
                    return (seq_t)pal;   /* cast to seq_t (primitive or big int) */
                }
            }
        }
    }
}
