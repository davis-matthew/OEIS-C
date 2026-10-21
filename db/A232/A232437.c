/*
 * A232437  Numbers whose square is expressible in only one way as x^2+xy+y^2,
 *          with x and y > 0.
 * https://oeis.org/A232437
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 103, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 *  OEIS A232437 - Numbers whose square is expressible in only one way as
 *  x^2 + xy + y^2, with x and y > 0.
 *
 *  Characterisation:
 *    n has exactly one prime divisor ≡ 1 (mod 6), that prime occurs with
 *    exponent 1, and no other prime divisor ≡ 1 (mod 6) occurs.
 *
 *  The function returns the n‑th such number (1‑based indexing),
 *  computing terms lazily and memoising results between calls.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static uint32_t *spf = NULL;         /* smallest prime factor array */
static size_t spf_size = 0;          /* current allocated size of spf */

static uint32_t *primes = NULL;      /* list of primes discovered so far */
static size_t primes_cap = 0;        /* capacity of primes array  */
static size_t primes_cnt = 0;        /* number of primes stored      */

static seq_t *seq = NULL;            /* memoised sequence values */
static size_t seq_cap = 0;           /* capacity of seq array       */
static size_t seq_size = 0;          /* number of terms stored      */

static size_t processed = 0;         /* last integer processed (>=0) */

/* Extend the sieve to compute spf up to new_limit (inclusive). */
static void extend_spf_to(size_t new_limit)
{
    if (new_limit <= spf_size)
        return;

    /* Resize spf array */
    size_t old_size = spf_size;
    size_t new_size = new_limit + 1;                 /* indices 0..new_limit */
    spf = (uint32_t *)realloc(spf, new_size * sizeof(uint32_t));
    memset(spf + old_size + 1, 0, (new_limit - old_size) * sizeof(uint32_t));
    spf_size = new_size;

    /* Linear sieve: compute spf and list of primes up to new_limit */
    for (size_t i = 2; i <= new_limit; ++i) {
        if (spf[i] == 0) {                           /* prime detected */
            spf[i] = (uint32_t)i;
            if (primes_cnt == primes_cap) {
                size_t new_cap = primes_cap == 0 ? 64 : primes_cap * 2;
                primes = (uint32_t *)realloc(primes, new_cap * sizeof(uint32_t));
                primes_cap = new_cap;
            }
            primes[primes_cnt++] = (uint32_t)i;
        }
        for (size_t j = 0; j < primes_cnt; ++j) {
            uint32_t p = primes[j];
            if (p > spf[i])
                break;
            size_t prod = i * (size_t)p;
            if (prod > new_limit)
                break;
            spf[prod] = p;
        }
    }
}

/* Append a number to the memoised sequence, expanding storage if needed. */
static void append_seq(size_t val)
{
    if (seq_size == seq_cap) {
        size_t new_cap = seq_cap == 0 ? 64 : seq_cap * 2;
        seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        seq_cap = new_cap;
    }
    seq[seq_size++] = (seq_t)val;
}

/* Test whether number num satisfies the sequence property. */
static bool good_number(size_t num)
{
    size_t temp = num;
    int count_p1 = 0;           /* count of primes ≡ 1 (mod 6) with exponent 1 */

    while (temp > 1) {
        uint32_t p = spf[temp];
        int exp = 0;
        while (temp % p == 0) {
            temp /= p;
            ++exp;
        }
        if (p % 6 == 1) {
            if (exp != 1)          /* exponent not 1 -> reject */
                return false;
            ++count_p1;
            if (count_p1 > 1)      /* more than one such prime -> reject */
                return false;
        }
    }
    return count_p1 == 1;
}

/* Main function to be called by the tests. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;          /* undefined for n < 1 */

    /* Initialisation on first call */
    if (spf_size == 0) {
        /* Setup initial sieve size */
        size_t init_limit = 1024;
        extend_spf_to(init_limit);
    }

    /* Generate terms until we have at least n of them */
    while (seq_size < (size_t)n) {
        /* Choose a new limit to process more integers */
        size_t new_limit = processed * 2 + 1000;
        if (new_limit <= processed)
            new_limit = processed + 1000;
        extend_spf_to(new_limit);

        for (size_t num = processed + 1; num <= new_limit; ++num) {
            if (good_number(num))
                append_seq(num);
        }
        processed = new_limit;
    }

    return seq[(size_t)n - 1];
}
