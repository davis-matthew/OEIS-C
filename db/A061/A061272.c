/*
 * A061272  Squares such that (1) each digit is a square, (2) the sum of
 *          squares of the digits is a square.
 * https://oeis.org/A061272
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A061272 – squares whose every decimal digit is a square digit
   (0,1,4,9) and whose digit‑square sum is also a square.
   a(1)=0, a(2)=1, a(3)=4, ...  (OEIS definitions) */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- integer square root on seq_t ----------
   Returns floor(sqrt(x)). Works for any seq_t that can store
   the value of x (no intermediate overflow). */
static seq_t isqrt_seq(seq_t x)
{
    if (x == 0 || x == 1)
        return x;
    seq_t low = 1, high = x / 2 + 1, mid, ans = 1;
    while (low <= high) {
        mid = low + ((high - low) >> 1);          /* floor division by 2   */
        seq_t sq = mid * mid;                     /* fits in seq_t         */
        if (sq == x)
            return mid;
        if (sq < x) {
            ans = mid;
            low = mid + 1;
        } else
            high = mid - 1;
    }
    return ans;
}

/* ---------- check that all digits are 0,1,4,9 and
   the sum of squares of digits is a perfect square ---------- */
static bool valid_square(seq_t v)
{
    if (v == 0)          /* 0 is special – has no digits in the loop */
        return true;

    seq_t temp = v;
    seq_t sum_sq = 0;
    while (temp > 0) {
        int d = (int)(temp % 10);
        if (d != 0 && d != 1 && d != 4 && d != 9)
            return false;
        sum_sq += (seq_t)d * (seq_t)d;
        temp /= 10;
    }
    seq_t r = isqrt_seq(sum_sq);
    return r * r == sum_sq;
}

/* ---------- cache of already found terms ----------
   The array is grown dynamically when more terms are needed. */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static unsigned long long next_root = 0;   /* next integer whose square to test */

/* ---------- compute a(n) ----------
   Ensures that cache contains at least n terms. ---------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* unused – offset is 1 */

    /* grow cache until it contains term n */
    while (cache_len < (size_t)n) {
        unsigned long long r = next_root++;        /* next root to test   */
        /* compute r*r into a 128‑bit intermediate to avoid overflow
           for seq_t up to 128 bits.  It will always fit into seq_t
           because the test is only performed for values that fit.    */
        unsigned __int128 sq128 = (unsigned __int128)r * (unsigned __int128)r;
        seq_t sq = (seq_t)sq128;

        if (valid_square(sq)) {
            /* grow array if needed */
            if (cache_len == 0 || cache_len == SIZE_MAX) {
                size_t new_cap = cache_len == 0 ? 64 : cache_len * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) exit(1);   /* out of memory */
                cache = tmp;
            }
            cache[cache_len++] = sq;
        }
    }
    return cache[n - 1];
}
