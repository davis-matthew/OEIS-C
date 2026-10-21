/*
 * A080359  The smallest integer x > 0 such that the number of primes in (x/2,
 *          x] equals n.
 * https://oeis.org/A080359
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Min{x > 0 | Pi(x) - Pi(floor(x/2)) = n}
 * Range:     u8 n <= 24, u16 n <= 3030, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The program precomputes a sieve and prime counts up to a fixed limit,
 * then searches x incrementally while keeping the minimal x that
 * achieves each difference value. */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static const size_t MAX_SIEVE = 5000000;          /* safe for n ≤ 10000 */
static unsigned char *is_prime = NULL;
static uint32_t *pi = NULL;
static seq_t *answers = NULL;                    /* 1‑based; index 0 unused */
static size_t answers_len = 1;                    /* current allocated size */
static size_t last_x = 1;                         /* last x processed */
static size_t last_answered_n = 0;                /* largest n for which an answer exists */

static void init_sieve(void)
{
    size_t i, j;
    is_prime = (unsigned char *)malloc(MAX_SIEVE + 1);
    pi = (uint32_t *)malloc((MAX_SIEVE + 1) * sizeof(uint32_t));
    for (i = 0; i <= MAX_SIEVE; ++i)
        is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;
    for (i = 2; i * i <= MAX_SIEVE; ++i)
        if (is_prime[i])
            for (j = i * i; j <= MAX_SIEVE; j += i)
                is_prime[j] = 0;
    pi[0] = 0;
    for (i = 1; i <= MAX_SIEVE; ++i)
        pi[i] = pi[i - 1] + is_prime[i];
    answers = (seq_t *)malloc(answers_len * sizeof(seq_t));
    answers[0] = 0;  /* unused */
}

/* Ensure answers vector can hold index n */
static void ensure_answers_size(idx_t n)
{
    if (n + 1 > answers_len) {
        size_t new_len = n + 10;               /* a small overhead */
        answers = (seq_t *)realloc(answers, new_len * sizeof(seq_t));
        memset(answers + answers_len, 0, (new_len - answers_len) * sizeof(seq_t));
        answers_len = new_len;
    }
}

seq_t a(idx_t n)
{
    static int init = 0;
    if (!init) {
        init_sieve();
        init = 1;
    }
    if (n == 0)
        return 0;  /* undefined, but return 0 for safety */
    ensure_answers_size(n);
    if (answers[n] != 0)
        return answers[n];

    /* Continue searching from the last processed x */
    for (size_t x = last_x + 1; ; ++x) {
        size_t diff = pi[x] - pi[x / 2];
        if (diff <= (size_t)n && answers[diff] == 0)
            answers[diff] = (seq_t)x;
        if (diff == (size_t)n) {
            last_x = x;
            return answers[n];
        }
    }
}
