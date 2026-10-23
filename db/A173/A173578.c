/*
 * A173578  a(n) = a(n-1) + A073053(a(n-1)).
 * https://oeis.org/A173578
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = a(n-1) + A073053(a(n-1)); a(1) = 1
 * Range:     u8 n <= 3, u16 n <= 254, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   allocate_seq_array, digit_len, pow10, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A073053 is DENEAT: concatenate the number of even digits, odd digits
   and total digit count of its argument. */

/* Required utilities */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/allocate_seq_array.h"
#include "../utils/seq_realloc.h"
#include "../utils/digit_len.h"
#include "../utils/pow10.h"

/* Helper: compute the DENEAT value of a number */
static seq_t deneat(seq_t x)
{
    seq_t e = 0, o = 0, d = 0;
    if (x == 0) {
        /* 0 has one digit, and it is even */
        e = 1;
        o = 0;
        d = 1;
    } else {
        seq_t tmp = x;
        while (tmp > 0) {
            seq_t digit = tmp % 10;
            if (digit % 2 == 0)
                e++;
            else
                o++;
            d++;
            tmp /= 10;
        }
    }

    /* Build the concatenated integer: e*10^(digits(o)+digits(d)) + o*10^digits(d) + d */
    idx_t  digits_o      = digit_len(o);
    idx_t  digits_d      = digit_len(d);
    idx_t  pow_exp       = digits_o + digits_d;
    seq_t  pow_o_d       = pow10(pow_exp);
    seq_t  pow_d         = pow10(digits_d);

    return e * pow_o_d + o * pow_d + d;
}

/* Main sequence function with memoisation */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined; callers use n>=1 */

    /* Static cache for already computed terms */
    static seq_t *cache   = NULL;
    static idx_t   cap    = 0;
    static idx_t   size   = 0; /* number of terms stored */

    /* Allocate initial cache if needed */
    if (cache == NULL) {
        cap   = n;
        cache = allocate_seq_array(cap);
        cache[0] = 1;   /* a(1) */
        size = 1;
    } else if (n > cap) {
        /* Grow cache to accommodate n terms */
        cap   = n;
        cache = seq_realloc(cache, size, cap);
    }

    /* Compute any missing terms up to n */
    for (idx_t i = size; i < n; ++i) {
        seq_t prev = cache[i - 1];
        seq_t add  = deneat(prev);
        cache[i]  = prev + add;
    }

    size = n;
    return cache[n - 1];
}
