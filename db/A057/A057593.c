/*
 * A057593  Triangle T(n, k) giving period length of the periodic sequence k^i
 *          (i >= imin) mod n (n >= 2, 1 <= k <= n-1).
 * https://oeis.org/A057593
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabl, nice
 * Formula:   a(n) is the period length of the sequence k^i (i ≥ imin) modulo
 *            n, where (n,k) is the flattened triangle index: rows start at
 *            n=2 with k from 1 to n‑1
 * Range:     u8 n <= 9737, u16 n <= 9737, u32 n <= 9737, u64 n <= 9737,
 *            u128 n <= 9737, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* multiply two 64‑bit numbers modulo mod, using 128‑bit arithmetic
   so we avoid overflow for any idx_t = uint64_t. */
static inline idx_t mul_mod(idx_t a, idx_t b, idx_t mod)
{
    return (idx_t)((unsigned __int128)a * b % mod);
}

/* Find the row r (r ≥ 2) such that the number of elements up to row r‑1
   is < s ≤ the number of elements up to row r.  Here s = n‑1 ≥ 1. */
static idx_t find_row(idx_t s)
{
    idx_t low = 2, high = 2;
    /* Exponential search for an upper bound. */
    while ((unsigned __int128)high * (high - 1) / 2 < s)
        high <<= 1;
    /* Binary search in [low, high] for the first r with cumulative ≥ s. */
    while (low + 1 < high)
    {
        idx_t mid = low + (high - low) / 2;
        if ((unsigned __int128)mid * (mid - 1) / 2 < s)
            low = mid;
        else
            high = mid;
    }
    return high;
}

/* Compute the period length T(r,k) of k^i (i ≥ imin) modulo r.
   Uses Floyd’s cycle‑finding algorithm, which works for any k
   with 1 ≤ k < r (r ≥ 2). */
static seq_t period_len(idx_t r, idx_t k)
{
    if (r < 2) return 0;
    idx_t mod   = r;
    idx_t kmod  = k % mod;      /* just in case k == mod, never happens */

    /* Step 1: find first intersection of tortoise and hare. */
    idx_t tort = mul_mod(1 % mod, kmod, mod);                 /* f(1) */
    idx_t hare = mul_mod(mul_mod(1 % mod, kmod, mod), kmod, mod); /* f(f(1)) */

    while (tort != hare)
    {
        tort = mul_mod(tort, kmod, mod);
        hare = mul_mod(mul_mod(hare, kmod, mod), kmod, mod);
    }

    /* Step 2: find start of cycle (mu). */
    idx_t tort2 = 1 % mod;
    idx_t mu = 0;
    while (tort2 != hare)
    {
        tort2 = mul_mod(tort2, kmod, mod);
        hare  = mul_mod(hare,  kmod, mod);
        ++mu;
    }

    /* Step 3: find cycle length (lam). */
    idx_t lam = 1;
    hare = mul_mod(tort2, kmod, mod);   /* first step from start of cycle */
    while (tort2 != hare)
    {
        hare = mul_mod(hare, kmod, mod);
        ++lam;
    }

    return (seq_t)lam;
}

/* Public sequence function.  n ≥ 2. */
seq_t a(idx_t n)
{
    if (n < 2) return 0;            /* undefined for offset < 2 */

    idx_t s = n - 1;                /* shift to 1‑based triangle position */

    idx_t r = find_row(s);                          /* row number */
    idx_t prev = (r - 2) * (r - 1) / 2;              /* cumulative count before row r */
    idx_t k = n - prev - 1;                          /* column (1‑based) in row r */

    return period_len(r, k);
}
