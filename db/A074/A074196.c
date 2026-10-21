/*
 * A074196  Sum of palindromes from the smallest prime factor of n to the
 *          largest prime factor of n.
 * https://oeis.org/A074196
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = sum of all palindromic numbers between the smallest prime
 *            * factor of n and the largest prime factor of n inclusive
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

static bool initialized = false;
static size_t max_limit = 0;
static seq_t *spf = NULL;      /* smallest prime factor */
static seq_t *lpfmax = NULL;   /* largest prime factor */
static uint8_t *is_pal = NULL; /* palindrome flag */

/* Resize and compute helper tables up to at least n. */
static void ensure_capacity(idx_t n)
{
    if (!initialized || (size_t)n > max_limit) {
        size_t old = max_limit;
        max_limit = (size_t)n + 100;   /* small buffer */
        spf = (seq_t *)realloc(spf, (max_limit + 1) * sizeof(seq_t));
        lpfmax = (seq_t *)realloc(lpfmax, (max_limit + 1) * sizeof(seq_t));
        is_pal = (uint8_t *)realloc(is_pal, (max_limit + 1) * sizeof(uint8_t));
        for (size_t i = old + 1; i <= max_limit; ++i) {
            spf[i] = 0;
            lpfmax[i] = 0;
            is_pal[i] = 0;
        }
        /* Sieve to fill spf and lpfmax. */
        for (size_t i = 2; i <= max_limit; ++i) {
            if (spf[i] == 0) {            /* i is prime */
                spf[i] = (seq_t)i;
                lpfmax[i] = (seq_t)i;
                for (size_t j = i; j <= max_limit; j += i) {
                    if (spf[j] == 0) spf[j] = (seq_t)i;
                    lpfmax[j] = (seq_t)i; /* later larger primes override */
                }
            }
        }
        /* Compute palindrome flags. */
        for (size_t i = 0; i <= max_limit; ++i) {
            size_t x = i;
            size_t rev = 0;
            size_t temp = x;
            while (temp > 0) {
                rev = rev * 10 + temp % 10;
                temp /= 10;
            }
            is_pal[i] = (rev == x);
        }
        initialized = true;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    if (n == 1) return (seq_t)0;
    ensure_capacity(n);
    seq_t lpf = spf[(size_t)n];
    seq_t lpfmax_val = lpfmax[(size_t)n];
    seq_t sum = 0;
    /* Sum all palindromic numbers in [lpf, lpfmax_val]. */
    for (size_t i = (size_t)lpf; i <= (size_t)lpfmax_val; ++i) {
        if (is_pal[i]) sum += (seq_t)i;
    }
    return sum;
}
