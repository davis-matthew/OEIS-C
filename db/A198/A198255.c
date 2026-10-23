/*
 * A198255  Number of ways to write n as the sum of two coprime squarefree
 *          semiprimes.
 * https://oeis.org/A198255
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = number of unordered representations of n as * the sum of
 *            two coprime squarefree semiprimes. (OEIS A198255)
 * Range:     u8 n <= 1327, u16 n <= 9339, u32 n <= 9339, u64 n <= 9339,
 *            u128 n <= 9339, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"

static idx_t max_n = 0;
static uint32_t *spf = NULL;      /* smallest prime factor */
static uint8_t  *is_semip = NULL; /* 1 if i is squarefree semiprime */

/* Ensure that spf and is_semip arrays are valid up to at least n. */
static void ensure_up_to(idx_t n)
{
    if (n <= max_n) return;

    /* Free old arrays if any. */
    if (spf)   free(spf);
    if (is_semip) free(is_semip);

    /* Allocate new arrays. */
    spf      = (uint32_t *)malloc((n + 1) * sizeof(uint32_t));
    is_semip = (uint8_t  *)malloc((n + 1) * sizeof(uint8_t));
    if (!spf || !is_semip) {
        /* allocation failed – abort calculation, return 0. */
        return;
    }

    /* Initialize arrays. */
    for (idx_t i = 0; i <= n; ++i) {
        spf[i]      = 0;
        is_semip[i] = 0;
    }

    /* Linear sieve for smallest prime factor. */
    for (idx_t i = 2; i <= n; ++i) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            /* Avoid overflow on i*i. */
            if ((unsigned long long)i * (unsigned long long)i <= (unsigned long long)n) {
                for (idx_t j = i * i; j <= n; j += i) {
                    if (spf[j] == 0) spf[j] = (uint32_t)i;
                }
            }
        }
    }

    /* Mark squarefree semiprimes: exactly two distinct prime factors. */
    for (idx_t i = 2; i <= n; ++i) {
        if (spf[i] != (uint32_t)i) {          /* composite */
            idx_t p = spf[i];
            idx_t q = i / p;
            if (q != p && spf[q] == (uint32_t)q) {
                is_semip[i] = 1;
            }
        }
    }

    max_n = n;
}

seq_t a(idx_t n)
{
    if (n < 6) return 0;          /* smallest possible sum is 6+10=16,
                                    but for n<6 there is no pair anyway. */

    ensure_up_to(n);
    if (!spf || !is_semip) return 0; /* allocation failed */

    seq_t count = 0;
    for (idx_t i = 2; i <= n / 2; ++i) {
        if (!is_semip[i]) continue;
        idx_t j = n - i;
        if (j <= 1 || !is_semip[j]) continue;
        if (gcd((seq_t)i, (seq_t)j) == 1) {
            ++count;
        }
    }
    return count;
}
