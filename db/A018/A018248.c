/*
 * A018248  The 10-adic integer x = ...1787109376 satisfies x^2 = x.
 * https://oeis.org/A018248
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 9999, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A018248 – digits of the 10‑adic integer x with x^2 = x.
   The digits are produced iteratively.  Let a[i] be the i‑th digit
   (a[0] = 6).  For i ≥ 1 let S_i be the convolution sum
      S_i = Σ_{j=0}^i a[j]·a[i-j]  (after the digit a[i] has been chosen)
   and let C_i be the carry that comes into position i from the lower
   digits.  Then a[i] satisfies

          a[i] ≡ (10 - (C_i + S_i) mod 10)  (mod 10)

   and the carry into the next position is
          C_{i+1} = (C_i + S_i) / 10 .

   Initially C_0 = 0 and S_0 = a[0]·a[0] = 36.  The algorithm uses
   only 32‑bit arithmetic for the convolution sums, which is ample for
   10 000 digits. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* ------------------------------------------------------------------ */
/* static scratch space – kept between calls so that any order of
   a(n) is handled efficiently.                                      */
/* ------------------------------------------------------------------ */
static uint8_t  *digit_cache  = NULL;      /* a[0] … a[<cached_len>] */
static uint32_t *sum_cache    = NULL;      /* convolution sums 0 … 2*cached_len-1 */
static idx_t    cached_len    = 0;          /* number of cached digits   */
static uint32_t cached_carry  = 0;          /* carry into position cached_len */

/* ------------------------------------------------------------------ */
/* Expand the cache so that digits up to index n are known.           */
/* ------------------------------------------------------------------ */
static void ensure_upto(idx_t n)
{
    /* Allocate more space if the requested index is beyond the cache. */
    if (cached_len <= n)
    {
        size_t new_len = n + 1;                         /* number of digits after expansion */
        /* grow digit array */
        digit_cache = (uint8_t *)realloc(digit_cache, new_len);
        for (size_t i = cached_len; i < new_len; ++i) digit_cache[i] = 0;

        /* grow convolution array */
        size_t old_sum_size = 2 * cached_len + 1;       /* elements before re‑alloc */
        size_t new_sum_size = 2 * new_len + 1;
        sum_cache = (uint32_t *)realloc(sum_cache,
                                        new_sum_size * sizeof(uint32_t));
        for (size_t i = old_sum_size; i < new_sum_size; ++i) sum_cache[i] = 0;

        /* compute new digits one by one */
        for (idx_t i = cached_len; i <= n; ++i)
        {
            uint32_t k = (cached_carry + sum_cache[i]) % 10;
            uint8_t  next_digit = (uint8_t)((10 - k) % 10);
            digit_cache[i] = next_digit;

            /* update convolution sums for positions >= i */
            for (idx_t j = 0; j < i; ++j)
                sum_cache[i + j] += 2U * (uint32_t)next_digit *
                                   (uint32_t)digit_cache[j];
            sum_cache[2 * i] += (uint32_t)next_digit *
                               (uint32_t)next_digit;

            /* compute carry into next position */
            uint32_t raw = sum_cache[i];
            cached_carry = (cached_carry + raw) / 10;
        }

        cached_len = new_len;
    }
}

/* ------------------------------------------------------------------ */
/* Public interface – required by the judge.                           */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Initialise on first call. */
    if (digit_cache == NULL)
    {
        cached_len = 1;
        digit_cache = (uint8_t *)malloc(cached_len);
        digit_cache[0] = 6; /* a(0) = 6 */

        sum_cache = (uint32_t *)calloc(2 * cached_len + 1, sizeof(uint32_t));
        sum_cache[0] = 36;            /* 6 * 6 */
        cached_carry = sum_cache[0] / 10;   /* carry into position 1 */
    }

    /* Compute missing digits up to the requested index. */
    ensure_upto(n);

    return (seq_t)digit_cache[n];
}
