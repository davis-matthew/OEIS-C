/*
 * A106146  Semiprimes (mod 10).
 * https://oeis.org/A106146
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = last digit of the n‑th semiprime (product of two primes)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: generate semiprimes by sieving smallest prime factors and
 * store their last digits in a dynamic array.  The array is extended
 * lazily as larger n are requested.                               */

#include <stdlib.h>

static seq_t *digits = NULL;          /* stored last digits of semiprimes */
static idx_t  stored = 0;             /* number of digits stored so far   */
static idx_t  digits_cap = 0;         /* current capacity of `digits`      */
static idx_t  prev_limit = 0;         /* largest number for which the sieve
                                        * has already been completed       */

/* enlarge the digits array if necessary */
static void ensure_digits_capacity(idx_t need)
{
    if (need <= digits_cap) return;
    idx_t new_cap = digits_cap ? digits_cap * 2 : 16;
    while (new_cap < need) new_cap <<= 1;
    seq_t *p = (seq_t *)realloc(digits, new_cap * sizeof(seq_t));
    digits = p;
    digits_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for n < 1 */

    /* increase the sieve size until we have at least n semiprimes */
    while (stored < n) {
        idx_t new_limit = prev_limit ? prev_limit * 2 : 200; /* start at 200 */
        if (new_limit == 0) new_limit = 200;

        /* allocate arrays for sieve up to new_limit */
        unsigned int  *spf = (unsigned int *)malloc((new_limit + 1) * sizeof(unsigned int));
        unsigned char *cnt = (unsigned char *)malloc((new_limit + 1) * sizeof(unsigned char));
        if (!spf || !cnt) {               /* allocation failure */
            free(spf); free(cnt);
            return 0;                     /* return 0 on error */
        }

        /* initialise spf to 0 */
        for (idx_t i = 0; i <= new_limit; ++i) spf[i] = 0;

        /* sieve for smallest prime factor */
        for (idx_t i = 2; i <= new_limit; ++i) {
            if (spf[i] == 0) {
                spf[i] = (unsigned int)i;
                if ((unsigned long long)i * i <= new_limit) {
                    for (idx_t j = i * i; j <= new_limit; j += i) {
                        if (spf[j] == 0) spf[j] = (unsigned int)i;
                    }
                }
            }
        }

        /* compute number of prime factors with multiplicity */
        cnt[0] = 0;   /* unused */
        cnt[1] = 0;
        for (idx_t i = 2; i <= new_limit; ++i) {
            cnt[i] = cnt[i / spf[i]] + 1;
        }

        /* add semiprimes found in (prev_limit, new_limit] */
        ensure_digits_capacity(stored + 1); /* capacity for at least one more */
        for (idx_t i = (prev_limit >= 2 ? prev_limit : 1) + 1; i <= new_limit; ++i) {
            if (cnt[i] == 2) {
                /* store last digit */
                digits[stored] = (seq_t)(i % 10);
                ++stored;
                ensure_digits_capacity(stored + 1);
            }
        }

        /* cleanup */
        free(spf);
        free(cnt);
        prev_limit = new_limit;
    }

    return digits[n - 1];
}
