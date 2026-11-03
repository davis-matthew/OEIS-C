/*
 * A189529  a(n) = n + [n*r/t] + [n*s/t]; r=1, s=sqrt(3)-sqrt(2),
 *          t=sqrt(3)+sqrt(2).
 * https://oeis.org/A189529
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + ⌊ n / (√3 + √2) ⌋ + ⌊ n / (5 + 2√6) ⌋ (with offset 1,
 *            so a(1) = 1)
 * Range:     u8 n <= 180, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 102 terms through n = 9939 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Test whether a candidate k satisfies k ≤ floor( n / γ ),
   where γ = 5 + 2√6 = τ².  The inequality
   follows from (n² – 49k²)² ≥ 2400k⁴ and n² ≥ 49k². */
static bool gamma_le(idx_t n, idx_t k)
{
    if (k == 0) return true;
    unsigned __int128 A = (unsigned __int128)n * n;   /* n²   */
    unsigned __int128 B = (unsigned __int128)k * k;   /* k²   */
    if (A < 49 * B) return false;                    /* n² ≥ 49k²  */
    unsigned __int128 diff = A - 49 * B;              /* n² – 49k²   */
    unsigned __int128 left  = diff * diff;           /* (n²-49k²)²  */
    unsigned __int128 right = 2400 * B * B;           /* 2400k⁴       */
    return left >= right;                            /* inequality */
}

/* Compute floor( n / γ ) by binary search using gamma_le. */
static idx_t floor_div_gamma(idx_t n)
{
    idx_t low  = 0;
    idx_t high = n / 9 + 1;    /* γ > 9  ⇒  floor(n/γ) ≤ n/9  */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (gamma_le(n, mid))
            low = mid + 1;
        else
            high = mid;
    }
    return low - 1;
}

/* Test whether a candidate k satisfies k ≤ floor( n / τ ),
   where τ = √3 + √2.  The inequality is equivalent to the
   two integer conditions below, which hold exactly for all
   0 ≤ k ≤ floor(n/τ).  */
static bool tau_le(idx_t n, idx_t k)
{
    if (k == 0) return true;
    unsigned __int128 A = (unsigned __int128)n * n;   /* n²   */
    unsigned __int128 B = (unsigned __int128)k * k;   /* k²   */
    if (A < 5 * B) return false;                     /* n² ≥ 5k²  */
    unsigned __int128 diff = A - 5 * B;               /* n² - 5k²   */
    unsigned __int128 left  = diff * diff;           /* (n²-5k²)²  */
    unsigned __int128 right = 24 * B * B;            /* 24k⁴       */
    return left >= right;                            /* inequality */
}

/* Compute floor( n / τ ) by binary search using tau_le. */
static idx_t floor_div_tau(idx_t n)
{
    idx_t low  = 0;
    idx_t high = n / 3 + 1;    /* τ > 3  ⇒  floor(n/τ) ≤ n/3  */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (tau_le(n, mid))
            low = mid + 1;
        else
            high = mid;
    }
    return low - 1;
}

/* Main sequence function required by the framework. */
seq_t a(idx_t n)
{
    seq_t res = (seq_t)n;
    res += (seq_t)floor_div_tau(n);
    res += (seq_t)floor_div_gamma(n);
    return res;
}
