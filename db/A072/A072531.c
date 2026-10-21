/*
 * A072531  Number of primes p < n such that n divided by p leaves a 1 or a
 *          composite (nonzero) remainder.
 * https://oeis.org/A072531
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #primes p < n such that n % p == 1 or (n % p > 1 and
 *            composite)
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* static tables for sieve */
static char *prime_comp = NULL;   /* prime_comp[i] == 1  → composite or 0/1, 0 → prime   */
static unsigned *prime_list = NULL;
static size_t prime_cnt = 0;
static size_t sieve_limit = 0;

static void extend_sieve(size_t limit)
{
    if (limit <= sieve_limit) return;

    char *tmp = (char *)realloc(prime_comp, limit + 1);
    if (!tmp) exit(1);
    prime_comp = tmp;

    /* initialise whole array */
    for (size_t i = 0; i <= limit; ++i) prime_comp[i] = 0;
    if (limit >= 0) prime_comp[0] = 1;
    if (limit >= 1) prime_comp[1] = 1;

    /* classic Eratosthenes */
    for (size_t i = 2; i * i <= limit; ++i)
        if (!prime_comp[i])
            for (size_t j = i * i; j <= limit; j += i)
                prime_comp[j] = 1;

    /* rebuild prime list */
    size_t cnt = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (!prime_comp[i]) ++cnt;

    unsigned *list = (unsigned *)realloc(prime_list, cnt * sizeof(unsigned));
    if (!list) exit(1);
    prime_list = list;

    size_t idx = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (!prime_comp[i]) prime_list[idx++] = (unsigned)i;

    prime_cnt   = cnt;
    sieve_limit = limit;
}

seq_t a(idx_t n)
{
    if (n <= 1) return 0;

    /* we need primes < n and prime tests up to n‑1 */
    extend_sieve((size_t)(n - 1));

    seq_t count = 0;
    for (size_t i = 0; i < prime_cnt && prime_list[i] < n; ++i) {
        idx_t r = n % (idx_t)prime_list[i];
        if (r == 1) ++count;                     /* remainder 1 is counted   */
        else if (r > 1 && prime_comp[(size_t)r]) ++count;  /* remainder composite   */
    }
    return count;
}
