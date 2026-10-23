/*
 * A156703  String of digits encountered in decimal expansion of successive
 *          ratios k/(k+1), treating only non-repeating expansions, with
 *          decimal point and leading and trailing zeros removed.
 * https://oeis.org/A156703
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = 10^d * (k-1) / k where k = A003592(n+1) = 2^i * 5^j and d
 *            = max(i, j)
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 28, u64 n <= 100, u128 n <= 355,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* local power function for unsigned base and exponent */
static seq_t seq_pow_uint(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp) {
        if (exp & 1u) result *= base;
        if (exp > 1u) base *= base;
        exp >>= 1u;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for n<1, guard for safety */

    /* static cache of A003592 (Hamming numbers 2^i 5^j) */
    static seq_t *h = NULL;
    static size_t h_cap = 0;
    static size_t h_len = 0;
    static size_t ptr2 = 0, ptr5 = 0;
    static seq_t next2 = 0, next5 = 0;

    /* Determine the required index (k = A003592(n+1) => use h[n]) */
    size_t target = (size_t)n;             /* since h[0] = 1 corresponds to k=1 */

    /* Ensure base array exists */
    if (h_len == 0) {
        h_cap = 16;
        h = (seq_t*)malloc(h_cap * sizeof(seq_t));
        if (!h) return 0;                  /* allocation failure guard */
        h[0] = 1;
        h_len = 1;
        ptr2 = 0;
        ptr5 = 0;
        next2 = h[ptr2] * 2;
        next5 = h[ptr5] * 5;
    }

    /* Extend cache up to the needed index if necessary */
    if (target >= h_len) {
        if (target + 1 > h_cap) {
            size_t new_cap = h_cap > 0 ? h_cap : 16;
            while (new_cap <= target + 1) new_cap <<= 1;
            seq_t *tmp = (seq_t*)realloc(h, new_cap * sizeof(seq_t));
            if (!tmp) return 0;             /* allocation failure guard */
            h = tmp;
            h_cap = new_cap;
        }
        for (size_t i = h_len; i <= target; ++i) {
            seq_t val = (next2 < next5) ? next2 : next5;
            h[i] = val;
            if (val == next2) {
                ++ptr2;
                next2 = h[ptr2] * 2;
            }
            if (val == next5) {
                ++ptr5;
                next5 = h[ptr5] * 5;
            }
        }
        h_len = target + 1;
    }

    /* Now k = h[n] */
    seq_t k = h[n];

    /* Find exponents of 2 and 5 in k */
    idx_t exp2 = 0, exp5 = 0;
    seq_t tmp = k;
    while (tmp % 2 == 0) { tmp /= 2; ++exp2; }
    while (tmp % 5 == 0) { tmp /= 5; ++exp5; }

    /* d = max(i,j) */
    idx_t d = (exp2 > exp5) ? exp2 : exp5;
    idx_t diff2 = d - exp2;
    idx_t diff5 = d - exp5;

    /* Compute t = 2^(d-i) * 5^(d-j) */
    seq_t t = seq_pow_uint((seq_t)2, diff2) * seq_pow_uint((seq_t)5, diff5);

    /* Return 10^d * (k-1) / k = t * (k-1) */
    return t * (k - 1);
}
