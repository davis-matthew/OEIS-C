/*
 * A198790  Irregular table T(n,k) read by rows: Last survivor positions in
 *          Josephus problem for n numbers and a count of k, n >= 1, lcm(1, 2,
 *          3, ..., n) >= k >= 1.
 * https://oeis.org/A198790
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   a(n) = T(r,k) where r,k satisfy sum_{i<r} L(i) < n ≤ sum_{i≤r}
 *            L(i), L(i)=lcm(1,…,i); following the recurrence T(1,1)=1,
 *            T(n,k)=((T(n-1, ((k-1) mod L(n-1))+1)+k-1) mod n)+1. The
 *            implementation builds L(i) up to the required row and evaluates
 *            the target entry recursively
 * Range:     u8 n <= 6441, u16 n <= 6441, u32 n <= 6441, u64 n <= 6441,
 *            u128 n <= 6441, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* GCD for 64‑bit unsigned integers. */
static inline uint64_t _gcd_uint64(uint64_t a, uint64_t b)
{
    while (b) { uint64_t t = a % b; a = b; b = t; }
    return a;
}

/* LCM table for 1..i (1‑based). 0‑element is unused. */
static uint64_t *lcms = NULL;   /* lcms[1] = 1 */
static idx_t max_row = 0;       /* largest computed index */

/* Ensure that lcms[1..need] are computed.  The product is performed in
   128‑bit to detect overflow; overflowed values are never needed for
   idx_t indices up to 2^64−1. */
static void ensure_lcms(idx_t need)
{
    if (need <= max_row) return;
    lcms = (uint64_t *)realloc(lcms, (need + 1) * sizeof(uint64_t));
    for (idx_t i = max_row + 1; i <= need; ++i) {
        if (i == 1) {
            lcms[1] = 1;
        } else {
            uint64_t prev = lcms[i - 1];
            uint64_t g = _gcd_uint64(prev, i);
            __int128 prod = (__int128)(prev / g) * i;
            if (prod > (__int128)UINT64_MAX) {
                /* Further rows would overflow 64‑bit; they are never needed
                   for sequence indices ≤ 2^64−1.  Stop at the current one. */
                max_row = i - 1;
                break;
            }
            lcms[i] = (uint64_t)prod;
        }
    }
    if (need > max_row) max_row = need;
}

/* Recursive evaluation of T(n,k).  All arithmetic is performed with
   idx_t (64‑bit) to avoid overflow in the intermediate steps. */
static seq_t compute_T(idx_t n, idx_t k)
{
    if (n == 1) return (seq_t)1;
    idx_t prevL = lcms[n - 1];
    idx_t prev_k = ((k - 1) % prevL) + 1;
    seq_t prev_val = compute_T(n - 1, prev_k);
    idx_t temp = ((idx_t)prev_val + (k - 1)) % n;
    return (seq_t)(temp + 1);
}

/* -------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined, but safe */

    idx_t cumulative = 0;
    idx_t r = 1;

    while (1) {
        ensure_lcms(r);
        uint64_t L = lcms[r];
        __int128 next = (__int128)cumulative + L;
        if (next >= n) break;
        cumulative = (idx_t)next;   /* fits < 2^64 */
        ++r;
    }

    idx_t k = n - cumulative;  /* 1‑based position in row r */
    return compute_T(r, k);
}
