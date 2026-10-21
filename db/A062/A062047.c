/*
 * A062047  Sum of odd numbers between consecutive primes.
 * https://oeis.org/A062047
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (prime(n+1)+prime(n))*(prime(n+1)-prime(n)-2)/4
 * Range:     u8 n <= 23, u16 n <= 649, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Prime table: prime_list[0] = 2 = prime(1) */
static seq_t *prime_list = NULL;
static idx_t prime_count = 0;   /* number of primes stored */

/* Ensure at least 'need' primes are available (prime(1)..prime(need)) */
static void ensure_primes(idx_t need)
{
    if (prime_count >= need)
        return;

    /* Rough upper bound for nth prime: n*(ln n + ln ln n) for n>=6.
       Use a safe overestimate: n * 20 + 100. */
    idx_t limit = (need < 6) ? 15 : (idx_t)(need * 20 + 100);

    while (true) {
        unsigned char *sieve = (unsigned char *)malloc(limit + 1);
        if (!sieve) exit(1);
        for (idx_t i = 0; i <= limit; ++i)
            sieve[i] = 1;
        sieve[0] = sieve[1] = 0;

        for (idx_t i = 2; i * i <= limit; ++i)
            if (sieve[i])
                for (idx_t j = i * i; j <= limit; j += i)
                    sieve[j] = 0;

        idx_t cnt = 0;
        for (idx_t i = 2; i <= limit; ++i)
            if (sieve[i]) ++cnt;

        if (cnt >= need) {
            seq_t *newlist = (seq_t *)realloc(prime_list, sizeof(seq_t) * cnt);
            if (!newlist) exit(1);
            prime_list = newlist;
            prime_count = cnt;
            idx_t pos = 0;
            for (idx_t i = 2; i <= limit; ++i)
                if (sieve[i])
                    prime_list[pos++] = (seq_t)i;
            free(sieve);
            break;
        }
        free(sieve);
        limit *= 2;   /* not enough, increase bound */
    }
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;         /* undefined for n<1, but return 0 */

    if (n == 1)
        return 0;         /* special case – formula gives -5/4 → 0 in OEIS */

    /* Need prime(n) and prime(n+1) */
    ensure_primes(n + 1);

    seq_t p1 = prime_list[n - 1]; /* prime(n)   */
    seq_t p2 = prime_list[n];     /* prime(n+1) */

    seq_t sum  = p1 + p2;               /* always fits in seq_t */
    seq_t diff = p2 - p1 - (seq_t)2;    /* diff >= 0 for n >= 2 */

    return sum * diff / 4;
}
