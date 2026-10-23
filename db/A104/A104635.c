/*
 * A104635  Odd n such that 2*n+1 is prime.
 * https://oeis.org/A104635
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor(A002145(n) / 2) = (p_n - 1) / 2 where p_n is * the
 *            n-th prime congruent to 3 mod 4. The sequence lists the odd *
 *            integers k such that 2k + 1 is prime
 * Range:     u8 n <= 29, u16 n <= 4232, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/* static storage for the primes ≡ 3 (mod 4) */
static uint64_t *pr3 = NULL;   /* array of primes */
static idx_t    pr3_count = 0; /* number of primes stored */
static idx_t    pr3_cap   = 0; /* allocated capacity */
static idx_t    pr3_limit = 0; /* last sieved limit */

/* Build the list of primes ≡ 3 (mod 4) up to a limit that contains at
 * least `need` elements.  Repeatedly doubles the limit until enough
 * primes are found.  The list is stored in the static array `pr3`. */
static void build_primes3(idx_t need)
{
    if (pr3_count >= need)          /* already have enough primes   */
        return;

    idx_t limit = pr3_limit > 0 ? pr3_limit * 2 : 100; /* start size */

    while (1) {
        /* Sieve of Eratosthenes up to `limit`. */
        unsigned char *is_comp = (unsigned char *)malloc((size_t)limit + 1);
        if (!is_comp)                      /* allocation failed */
            return;
        memset(is_comp, 0, (size_t)limit + 1);

        for (uint64_t i = 2; i * i <= limit; ++i) {
            if (!is_comp[i]) {
                for (uint64_t j = i * i; j <= limit; j += i)
                    is_comp[j] = 1;
            }
        }

        /* Count how many primes ≡ 3 (mod 4) appear. */
        idx_t cnt = 0;
        for (uint64_t p = 3; p <= limit; p += 2) {
            if (!is_comp[p] && (p & 3) == 3)
                ++cnt;
        }

        if (cnt < need) {                 /* not enough – enlarge limit */
            free(is_comp);
            limit *= 2;
            continue;
        }

        /* Ensure the array is large enough. */
        if (pr3_cap < cnt) {
            uint64_t *tmp = (uint64_t *)realloc(pr3, sizeof(uint64_t) * cnt);
            if (!tmp) {                   /* reallocation failed */
                free(is_comp);
                return;
            }
            pr3 = tmp;
            pr3_cap = cnt;
        }

        /* Fill the array with the primes. */
        idx_t idx = 0;
        for (uint64_t p = 3; p <= limit; p += 2) {
            if (!is_comp[p] && (p & 3) == 3)
                pr3[idx++] = p;
        }

        pr3_count = cnt;
        pr3_limit = limit;
        free(is_comp);
        return;                            /* done */
    }
}

/* External function required by the framework. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* undefined for offset-0, but return 0 */
    build_primes3(n);            /* ensure we have at least n primes */
    seq_t p = (seq_t)pr3[n - 1];
    return ((p - (seq_t)1) >> 1); /* (p-1)/2 */
}
