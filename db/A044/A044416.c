/*
 * A044416  Numbers k such that string '84' occurs in the base 10
 *          representation of k but not of k-1.
 * https://oeis.org/A044416
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th positive integer k such that the decimal
 *            representation of k contains the substring "84" while k-1 does
 *            not
 * Range:     u8 n <= 2, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Validated: partial, pass, 39 terms through n = 39 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function computes terms lazily and caches them.               */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Test whether a non‑negative integer contains the substring "84".  */
static bool contains84_u64(uint64_t x)
{
    /* Store digits in reverse order (least significant first).  */
    unsigned int digits[20];          /* 20 digits are enough for 64‑bit numbers.    */
    int len = 0;
    do {
        digits[len++] = (unsigned int)(x % 10);
        x /= 10;
    } while (x > 0);

    /* Scan from most significant to least significant digit.      */
    for (int i = len - 1; i > 0; --i)
    {
        if (digits[i] == 8 && digits[i - 1] == 4)
            return true;
    }
    return false;
}

/* Cache of the first terms found; size sufficient for up to 10 000    
   pre‑computed terms (you can enlarge if you wish).                   */
#define CACHE_MAX 32768
static seq_t cache[CACHE_MAX];
static size_t cache_len = 0;

/* Next integer to examine when searching for further terms.        */
static uint64_t next_candidate = 84ULL;

/* -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Generate terms until we have at least n of them.                */
    while (cache_len < (size_t)n)
    {
        uint64_t k = next_candidate;
        while (1)
        {
            if (contains84_u64(k) && !contains84_u64(k - 1))
            {
                cache[cache_len++] = (seq_t)k;   /* cast truncates if needed */
                next_candidate = k + 1;
                break;
            }
            ++k;
        }
    }
    return cache[(size_t)(n - 1)];
}
