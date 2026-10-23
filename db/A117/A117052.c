/*
 * A117052  Enneagonal numbers for which the product of the digits is also an
 *          enneagonal number.
 * https://oeis.org/A117052
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 50, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   enneagonal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A117052 – Enneagonal numbers whose digit product is also enneagonal.
 *  This implementation generates enneagonal numbers in order and
 *  returns the n‑th number whose digit product is another enneagonal
 *  number.  No memoisation is used; each call performs an O(n) search,
 *  which is fast enough for n up to about 10000.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/enneagonal.h"

/* --- Helper: product of decimal digits ------------------------------------ */
static inline seq_t digit_product(seq_t x)
{
    if (x == (seq_t)0) return (seq_t)0;
    seq_t prod = (seq_t)1;
    while (x != (seq_t)0) {
        seq_t d = x % (seq_t)10;
        prod = prod * d;
        x   = x / (seq_t)10;
    }
    return prod;
}

/* --- Helper: integer floor square root (binary search) ------------------- */
static inline seq_t floor_sqrt(seq_t x)
{
    if (x == (seq_t)0) return (seq_t)0;
    seq_t lo = (seq_t)1;
    seq_t hi = x;
    seq_t ans = (seq_t)1;
    while (lo <= hi) {
        seq_t mid = (lo + hi) / (seq_t)2;
        seq_t sq  = mid * mid;
        if (sq == x) return mid;
        if (sq < x) {
            lo  = mid + (seq_t)1;
            ans = mid;
        } else {
            hi = mid - (seq_t)1;
        }
    }
    return ans;
}

/* --- Helper: check if a number is enneagonal ----------------------------- */
static bool is_enneagonal(seq_t n)
{
    if (n == (seq_t)0) return true;          /* E(0) = 0 */
    seq_t disc = (seq_t)25 + (seq_t)56 * n;  /* discriminant of 7m²-5m-2n = 0 */
    seq_t s    = floor_sqrt(disc);
    if (s * s != disc) return false;        /* not a perfect square */
    /* check integer root: (5 + s) must be divisible by 14 */
    return ((seq_t)5 + s) % (seq_t)14 == (seq_t)0;
}

/* --- Public function ----------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < (idx_t)1) return (seq_t)0;  /* undefined for n < 1 */

    idx_t found = (idx_t)0;
    idx_t k     = (idx_t)0;

    while (found < n) {
        seq_t e    = enneagonal(k);
        seq_t prod = digit_product(e);
        if (is_enneagonal(prod)) {
            ++found;
            if (found == n) return e;
        }
        ++k;
    }
    return (seq_t)0; /* should never reach here */
}
