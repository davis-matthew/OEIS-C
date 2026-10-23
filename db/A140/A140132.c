/*
 * A140132  a(n) =
 *          Sum_digits{a(n-1)+a(n-2)+Sum_digits[a(n-1)]+Sum_digits[a(n-2)]},
 *          with a(0)=0 and a(1)=1.
 * https://oeis.org/A140132
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = Sum_digits( a(n-1) + a(n-2) + Sum_digits[a(n-1)] +
 *            Sum_digits[a(n-2)] ), with a(0)=0, a(1)=1. The value is the sum
 *            of the decimal digits of the quantity in the parentheses
 * Range:     u8 n <= 94, u16 n <= 94, u32 n <= 94, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t digit_sum(seq_t x)
{
    seq_t sum = 0;
    while (x > 0) {
        sum += x % 10;
        x   /= 10;
    }
    return sum;
}

static seq_t *cache    = NULL;
static idx_t   cached   = 0;   /* number of values stored (<= cached)   */
static idx_t   capacity = 0;   /* allocated length                      */

static void ensure_capacity(idx_t needed)
{
    if (needed <= capacity)
        return;
    idx_t new_cap = capacity ? capacity << 1 : 4;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();
    cache    = tmp;
    capacity = new_cap;
}

seq_t a(idx_t n)
{
    /* Initialise base cases on first call */
    if (cached == 0) {
        ensure_capacity(2);
        cache[0] = 0;
        cache[1] = 1;
        cached   = 2;
    }

    if (n < cached)
        return cache[n];

    for (idx_t i = cached; i <= n; ++i) {
        seq_t a1 = cache[i-1];
        seq_t a0 = cache[i-2];
        seq_t s1 = digit_sum(a1);
        seq_t s0 = digit_sum(a0);
        seq_t t  = a1 + a0 + s1 + s0;
        seq_t val= digit_sum(t);
        ensure_capacity(i+1);
        cache[i] = val;
    }
    cached = n + 1;
    return cache[n];
}
