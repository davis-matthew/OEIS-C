/*
 * A213313  Numbers with exactly 6 nonprime substrings (substrings with
 *          leading zeros are considered to be nonprime).
 * https://oeis.org/A213313
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, base
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 2048, u32 n <= 2351, u64 n <= 2351,
 *            u128 n <= 2351, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) is the n‑th 8‑digit (≤10^7) integer that has exactly
 * six nonprime substrings. The values are computed by brute‑force
 * search using a prime sieve up to 10 000 000.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Prime sieve --------------------------------------------------------- */
static unsigned char *prime_flag = NULL;   /* prime_flag[i] = 1 if i is prime */
static int p_limit = 0;

/* Build a sieve of Eratosthenes up to 'limit'. */
static void build_prime_sieve(int limit)
{
    p_limit = limit;
    prime_flag = (unsigned char *)malloc((size_t)(limit + 1));
    if (!prime_flag) abort();
    for (int i = 0; i <= limit; ++i) prime_flag[i] = 1;
    if (limit >= 0) prime_flag[0] = 0;
    if (limit >= 1) prime_flag[1] = 0;
    for (int i = 2; (long long)i * i <= limit; ++i) {
        if (prime_flag[i]) {
            for (int j = i * i; j <= limit; j += i) prime_flag[j] = 0;
        }
    }
}

/* Count nonprime substrings of a decimal integer (num ≤ 10 000 000). */
static unsigned int count_nonprime_substrings(uint32_t num)
{
    /* Extract digits most‑significant first. */
    unsigned char d[8];
    int len = 0;
    uint32_t tmp = num;
    do {
        d[len++] = (unsigned char)(tmp % 10);
        tmp /= 10;
    } while (tmp);
    for (int i = 0, j = len - 1; i < j; ++i, --j) {
        unsigned char t = d[i];
        d[i] = d[j];
        d[j] = t;
    }

    unsigned cnt = 0;
    for (int i = 0; i < len; ++i) {
        if (d[i] == 0) {                 /* leading zero ⇒ nonprime */
            cnt += (unsigned)(len - i);
            if (cnt > 6) return cnt;
            continue;
        }
        int val = 0;
        for (int j = i; j < len; ++j) {
            val = val * 10 + d[j];
            if (val <= p_limit && prime_flag[val]) continue;
            ++cnt;
            if (cnt > 6) return cnt;
        }
    }
    return cnt;
}

/* Storage of the sequence ---------------------------------------------- */
static seq_t *terms = NULL;      /* array of found numbers */
static size_t term_len = 0;
static size_t term_cap = 0;

/* Grow the terms array when needed. */
static void grow_terms(void)
{
    if (term_cap < term_len + 1) {
        size_t newcap = term_cap ? term_cap * 2 : 16;
        while (newcap < term_len + 1) newcap <<= 1;
        seq_t *newptr = (seq_t *)realloc(terms, newcap * sizeof(seq_t));
        if (!newptr) abort();
        terms = newptr;
        term_cap = newcap;
    }
}

/* Build the entire sequence once at first call. */
static bool initialized = false;

static void build_sequence(void)
{
    const uint32_t MAX_NUM = 10000000;   /* boundary guaranteed <10^7 */
    build_prime_sieve(MAX_NUM);
    terms = NULL;
    term_len = 0;
    term_cap = 0;

    for (uint32_t num = 1; num <= MAX_NUM; ++num) {
        if (count_nonprime_substrings(num) == 6) {
            grow_terms();
            terms[term_len++] = (seq_t)num;
        }
    }
    initialized = true;
}

/* Return the n-th term (1‑based).  If n is beyond the sequence,
 * zero is returned. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety fall‑back, sequence starts at 1 */

    if (!initialized) build_sequence();

    if ((idx_t)term_len < n) return 0;   /* n beyond known terms */

    return terms[(size_t)n - 1];
}
