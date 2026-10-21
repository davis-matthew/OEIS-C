/*
 * A079123  Number of 4's in n# (n primorial) = 4's in A002110(n).
 * https://oeis.org/A079123
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number of '4' digits in the decimal representation of the
 *            product of the first n primes (primorial)
 * Range:     u8 n <= 707, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#define BASE         1000000000U   /* base for limb representation (10^9) */
#define PRIME_LIMIT  300000        /* upper bound for sieving primes */
#define PRIME_CAP    26000         /* sufficient prime count (~26k primes) */
#define BIG_CAP      12000         /* limb capacity for big integer */
#define MEMO_CAP     20001         /* maximum n handled (up to 20000) */

static uint32_t primes[PRIME_CAP];
static size_t   prime_count = 0;

static uint32_t big[BIG_CAP];
static size_t   big_len;          /* current limb count of big */

static seq_t memo[MEMO_CAP];
static size_t memo_len;           /* number of memoized terms (index+1) */

/* ------------------------------------------------------------ */
/* Prime sieve (Eratosthenes) to fill primes[]                     */
static void init_primes(void)
{
    bool *is_prime = (bool*)malloc((PRIME_LIMIT + 1) * sizeof(bool));
    for (int i = 0; i <= PRIME_LIMIT; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    for (int i = 2; (long long)i * i <= PRIME_LIMIT; ++i)
        if (is_prime[i])
            for (int j = i * i; j <= PRIME_LIMIT; j += i)
                is_prime[j] = false;

    for (int i = 2; i <= PRIME_LIMIT && prime_count < PRIME_CAP; ++i)
        if (is_prime[i]) primes[prime_count++] = (uint32_t)i;

    free(is_prime);
}

/* ------------------------------------------------------------ */
/* Multiply the big integer by a small prime                      */
static void multiply_big_by_prime(uint32_t prime)
{
    uint64_t carry = 0;
    for (size_t i = 0; i < big_len; ++i) {
        uint64_t cur   = (uint64_t)big[i] * prime + carry;
        big[i]         = (uint32_t)(cur % BASE);
        carry          = cur / BASE;
    }
    while (carry) {
        if (big_len >= BIG_CAP) { /* should never happen with our limits */
            return;
        }
        big[big_len++] = (uint32_t)(carry % BASE);
        carry /= BASE;
    }
}

/* ------------------------------------------------------------ */
/* Count '4' digits in the decimal representation of big        */
static int count_fours(void)
{
    int  cnt   = 0;
    /* most significant limb – no leading zeros added */
    uint32_t limb = big[big_len - 1];
    while (limb) {
        if ((limb % 10) == 4) ++cnt;
        limb /= 10;
    }
    /* remaining limbs – each represents exactly 9 decimal digits */
    for (size_t i = big_len - 1; i > 0; --i) {
        uint32_t l = big[i - 1];
        for (int k = 0; k < 9; ++k) {
            if ((l % 10) == 4) ++cnt;
            l /= 10;
        }
    }
    return cnt;
}

/* ------------------------------------------------------------ */
/* One‑time initialization routine                              */
static void init(void)
{
    init_primes();
    big[0]   = 1;    /* primorial of 0 primes = 1 */
    big_len  = 1;
    memo[0]  = 0;    /* a(0) */
    memo_len = 1;    /* indices 0..memo_len-1 cached */
}

/* ------------------------------------------------------------ */
/* Main function required by the problem statement             */
seq_t a(idx_t n)
{
    static bool initialized = false;
    if (!initialized) {
        init();
        initialized = true;
    }
    if (n >= MEMO_CAP) {
        /* n exceeds preallocated limits – not expected in tests */
        return 0;
    }
    /* Extend memo up to n if necessary */
    while (memo_len <= (size_t)n) {
        multiply_big_by_prime(primes[memo_len - 1]);     /* use next prime */
        int cnt4 = count_fours();
        memo[memo_len] = (seq_t)cnt4;
        ++memo_len;
    }
    return memo[n];
}
