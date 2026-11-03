/*
 * A232185  Let x(0)x(1)x(2)...x(q) denote the decimal expansion of k.
 *          Sequence lists all zeroless numbers k > 10 such that
 *          x(0)/x(1)x(2)... x(q) + x(0)x(1)/x(2)x(3)...x(q) + ... +
 *          x(0)x(1)... x(q-1)/x(q) is an integer.
 * https://oeis.org/A232185
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, more
 * Formula:   a(n) = (sequence of zeroless integers k > 10 such that *
 *            Σ_{i=0}^{q-1} (prefix_i)/(suffix_{i+1}) is an integer)
 * Range:     u8 n <= 23, u16 n <= 26, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Validated: partial, pass, 26 terms through n = 26 (2026-11-03)
 * Helpers:   gcd, mem, num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The algorithm performs a brute‑force search, computing the sum
 *  as a reduced rational number.  To avoid over‑large numbers
 *  the denominator is reduced at every step using gcd.  The
 *  suffix of the number is obtained via k % 10^m, so no
 *  huge intermediate products are formed.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

#include "../utils/gcd.h"
#include "../utils/num_digits.h"
#include "../utils/mem.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

/* --------------------------------------------------------------------- */
static bool has_zero(uint64_t x)
{
    while (x > 0) {
        if (x % 10 == 0)
            return true;
        x /= 10;
    }
    return false;
}

/* --------------------------------------------------------------------- */
static bool qualifies(uint64_t k)
{
    /* ---- 1. obtain digits most‑significant‑first  ------------------- */
    int digits[20];
    int len = 0;
    uint64_t tmp = k;
    while (tmp > 0) {
        digits[len++] = tmp % 10;
        tmp /= 10;
    }
    if (len <= 1)            /* we only need k > 10 */
        return false;

    for (int i = 0; i < len / 2; ++i) {
        int t = digits[i];
        digits[i] = digits[len - 1 - i];
        digits[len - 1 - i] = t;
    }

    /* ---- 2. evaluate the rational sum  --------------------------------- */
    seq_t prefix = 0;     /* current prefix as integer */
    seq_t num = 0;        /* numerator of the sum */
    seq_t den = 1;        /* denominator of the sum */

    int total_digits = num_digits((seq_t)k);

    for (int i = 0; i <= total_digits - 2; ++i) {
        /* build next prefix digit */
        prefix = prefix * 10 + (seq_t)digits[i];

        /* suffix = k % 10^(total_digits-i-1) */
        seq_t suf = ((seq_t)k) % pow10((idx_t)(total_digits - i - 1));

        /* add the fraction prefix/suf to the running sum */
        seq_t new_num = num * suf + prefix * den;
        seq_t new_den = den * suf;
        seq_t g = gcd(new_num, new_den);
        num = new_num / g;
        den = new_den / g;
    }
    /* the sum is integer iff the final denominator is 1 */
    return den == 1;
}

/* --------------------------------------------------------------------- */
/* Dynamic cache of found terms                                         */
static seq_t *cache      = NULL;   /* array of stored values */
static size_t cache_cap  = 0;     /* allocated capacity          */
static size_t cache_len  = 0;     /* number of stored values     */
static uint64_t last_checked = 10;/* last integer tested         */

/* --------------------------------------------------------------------- */
static void cache_add(seq_t val)
{
    if (cache_len == cache_cap) {                     /* grow cache    */
        size_t newcap = cache_cap == 0 ? 4 : cache_cap * 2;
        seq_t *tmp = mem_resize(seq_t, cache, newcap);
        if (!tmp) exit(1);                           /* allocation fail */
        cache = tmp;
        cache_cap = newcap;
    }
    cache[cache_len++] = val;
}

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                                 /* safety guard   */
    if (cache_len >= n) return cache[n - 1];              /* already known   */

    /* generate terms until we have n of them */
    while (cache_len < n) {
        ++last_checked;
        if (has_zero(last_checked))
            continue;                                     /* skip zeroless rule */
        if (qualifies(last_checked))
            cache_add((seq_t)last_checked);
    }
    return cache[n - 1];
}
