/*
 * A085379  Greatest prime as sum of distinct divisors of n.
 * https://oeis.org/A085379
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   Greatest prime that can be written as a sum of distinct divisors
 *            of n
 * Range:     u8 n <= 107, u16 n <= 9971, u32 n <= 9971, u64 n <= 9971,
 *            u128 n <= 9971, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* --- Prime sieve -------------------------------------------------------- */
static bool *prime_sieve = NULL;
static size_t sieve_limit = 1;

static void ensure_prime_sieve(size_t limit)
{
    if (prime_sieve && limit <= sieve_limit)
        return;

    free(prime_sieve);
    prime_sieve = (bool *)malloc((limit + 1) * sizeof(bool));
    for (size_t i = 0; i <= limit; ++i)
        prime_sieve[i] = true;
    if (limit >= 0) prime_sieve[0] = false;
    if (limit >= 1) prime_sieve[1] = false;

    for (size_t i = 2; i * i <= limit; ++i)
        if (prime_sieve[i]) {
            for (size_t j = i * i; j <= limit; j += i)
                prime_sieve[j] = false;
        }

    sieve_limit = limit;
}

/* --- Main sequence function --------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0; /* behavior below the offset is irrelevant */

    /* factor n and generate all divisors --------------------------------- */
    uint64_t un = (uint64_t)n;
    uint64_t temp = un;
    uint64_t primes[32];
    int exps[32];
    int pcnt = 0;

    uint64_t p;
    int exp;
    for (p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            primes[pcnt] = p;
            exps[pcnt++] = exp;
        }
    }
    if (temp > 1) {
        primes[pcnt] = temp;
        exps[pcnt++] = 1;
    }

    /* build list of divisors */
    uint64_t *divs = (uint64_t *)malloc(sizeof(uint64_t) * 256);
    size_t div_cnt = 1;
    divs[0] = 1;

    for (int fi = 0; fi < pcnt; ++fi) {
        uint64_t base = primes[fi];
        int e = exps[fi];
        size_t cur_sz = div_cnt;
        uint64_t power = 1;
        for (int k = 1; k <= e; ++k) {
            power *= base;
            for (size_t j = 0; j < cur_sz; ++j)
                divs[div_cnt++] = divs[j] * power;
        }
    }

    /* compute sigma(n) */
    uint64_t sum_total = 0;
    for (size_t i = 0; i < div_cnt; ++i)
        sum_total += divs[i];

    /* initialise prime sieve up to sigma(n) */
    ensure_prime_sieve((size_t)sum_total);

    /* subset‑sum bitset using bitwise DP ------------------------------- */
    size_t word_cnt = (sum_total >> 6) + 1;
    unsigned long long *bits =
        (unsigned long long *)calloc(word_cnt, sizeof(unsigned long long));
    bits[0] = 1ULL;           /* sum 0 reachable */

    for (size_t di = 0; di < div_cnt; ++di) {
        uint64_t d = divs[di];
        size_t word_shift = d >> 6;
        unsigned char bit_shift = (unsigned char)(d & 63);

        unsigned long long *src =
            (unsigned long long *)malloc(word_cnt * sizeof(unsigned long long));
        memcpy(src, bits, word_cnt * sizeof(unsigned long long));

        for (size_t i = word_cnt; i-- > word_shift; ) {
            unsigned long long val = src[i - word_shift] << bit_shift;
            if (bit_shift && (i - word_shift > 0))
                val |= src[i - word_shift - 1] >> (64 - bit_shift);
            bits[i] |= val;
        }

        free(src);
    }

    /* find largest reachable prime ------------------------------------- */
    seq_t result = 0;
    for (uint64_t m = sum_total; m >= 2; --m) {
        if (prime_sieve[m]) {
            if ((bits[m >> 6] >> (m & 63)) & 1ULL) {
                result = (seq_t)m;
                break;
            }
        }
        if (m == 0) break; /* safety against unsigned underflow */
    }

    free(bits);
    free(divs);
    return result;
}
