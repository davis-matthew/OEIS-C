/*
 * A140129  Let c(n) = 0 if n=1, (c(floor(n/2)) + c(floor((n+1)/2)))/2 if 1 <
 *          n < 3*2^floor(log_2(n)-1), and c(n-2^floor(log_2(n))) + 1
 *          otherwise. Then a(n) = numerator(c(n)).
 * https://oeis.org/A140129
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, look
 * Formula:   a(n) = numerator(c(n)) where c(1) = 0 c(n) =
 *            (c(n/2)+c((n+1)/2))/2 if 1 < n < 3·2^{⌊log₂(n)⌋−1} =
 *            c(n−2^{⌊log₂(n)⌋})+1 otherwise (n > 1) The function returns the
 *            numerator as an exact seq_t value
 * Range:     u8 n <= 66, u16 n <= 1896, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   floor_log2_uint64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*--------------------------------- Includes ---------------------------------*/
#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/floor_log2_uint64.h"

/*--------------------------------- Types -------------------------------------*/
typedef struct {
    seq_t num;
    seq_t den;
} fraction_t;

/*------------ Cache for small n (up to CACHE_LIMIT) ------------------------*/
#define CACHE_LIMIT 200000
static seq_t  num_cache[CACHE_LIMIT + 1];
static seq_t  den_cache[CACHE_LIMIT + 1];
static unsigned char computed[CACHE_LIMIT + 1] = {0};

/*--------- Recursively compute fraction for index n (no caching for n > limit) */
static fraction_t get_fraction(idx_t n)
{
    if (n == 1) {
        fraction_t f = { 0, 1 };
        if (n <= CACHE_LIMIT) {
            num_cache[n]  = 0;
            den_cache[n]  = 1;
            computed[n]   = 1;
        }
        return f;
    }

    /* Try cache if possible */
    if (n <= CACHE_LIMIT && computed[n]) {
        fraction_t f = { num_cache[n], den_cache[n] };
        return f;
    }

    /* Compute based on the recurrence */
    idx_t k        = floor_log2_uint64(n);          /* ⌊log₂ n⌋  */
    idx_t power2   = (idx_t)1 << k;                 /* 2^k       */
    idx_t thr      = 3 * ((idx_t)1 << (k - 1));     /* 3·2^{k-1} */

    fraction_t result;

    if (n > 1 && n < thr) {     /* averaging branch */
        fraction_t rl = get_fraction(n >> 1);            /* floor(n/2)   */
        fraction_t rr = get_fraction((n + 1) >> 1);      /* floor((n+1)/2) */

        fraction_t sum = { rl.num * rr.den + rr.num * rl.den,
                           rl.den * rr.den * 2 };      /* (a/b + c/d)/2 */

        seq_t g = gcd(sum.num, sum.den);
        result.num = sum.num / g;
        result.den = sum.den / g;
    } else {                    /* shift + 1 branch */
        fraction_t pr = get_fraction(n - power2);

        fraction_t sum = { pr.num + pr.den, pr.den };  /* (pr.num/pr.den)+1 */

        seq_t g = gcd(sum.num, sum.den);
        result.num = sum.num / g;
        result.den = sum.den / g;
    }

    /* Store in cache if within limits */
    if (n <= CACHE_LIMIT) {
        num_cache[n]  = result.num;
        den_cache[n]  = result.den;
        computed[n]   = 1;
    }

    return result;
}

/*------------------------------ Public interface ---------------------------*/
seq_t a(idx_t n)
{
    if (n == 0)               /* offset is 1, but guard anyway */
        return 0;
    fraction_t f = get_fraction(n);
    return f.num;
}
