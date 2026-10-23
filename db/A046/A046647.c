/*
 * A046647  Number of certain rooted planar maps.
 * https://oeis.org/A046647
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*(7*n-15)*(3*n-6)!/((n-2)!*(2*n-2)!)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 15, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   build_primes_upto, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/pow_seq.h"

/* --------- prime table (local to this file) --------- */
static uint64_t *prime_buf = NULL;
static size_t    prime_cnt = 0;
static bool      primes_inited = false;

static void init_primes(void)
{
    if (!primes_inited) {
        /* maximum needed: 3*10000-6 = 29994 → use 30000 */
        idx_t limit = 30000;
        uint64_t *tmp;
        size_t cnt;
        build_primes_upto(limit, &tmp, &cnt);
        prime_buf  = tmp;
        prime_cnt  = cnt;
        primes_inited = true;
    }
}

/* exponent of prime p in m! */
static seq_t exp_in_fact(idx_t m, idx_t p)
{
    seq_t e = 0;
    idx_t d = p;
    while (d <= m) {
        e += m / d;
        if (d > m / p) break;
        d *= p;
    }
    return e;
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* value below the offset is irrelevant   */
    if (n == 2) return 1;          /* a(2) = 1  (formula only holds for n≥3)*/

    seq_t result = 1;

    /* factors to be multiplied in the numerator */
    seq_t factor_val = 4 * ((seq_t)7 * n - 15);

    idx_t m     = 3 * n - 6;   /* top of the factorial          */
    idx_t d1    = n - 2;       /* (n-2)!                        */
    idx_t d2    = 2 * n - 2;   /* (2n-2)!                       */

    init_primes();

    for (size_t i = 0; i < prime_cnt && prime_buf[i] <= m; ++i) {
        idx_t p = (idx_t)prime_buf[i];

        seq_t exp_num   = exp_in_fact(m,   p);
        seq_t exp_den   = exp_in_fact(d1,  p) + exp_in_fact(d2,  p);
        seq_t exp_factor = 0;

        /* remove any power of p that appears in the explicit factor */
        while (factor_val % p == 0) {
            factor_val /= p;
            ++exp_factor;
        }

        seq_t exp_total = exp_num - exp_den + exp_factor;
        if (exp_total > 0) {
            result *= pow_seq((seq_t)p, (int)exp_total);
        }
        /* for n ≥ 3 exp_total is guaranteed non‑negative (factor cancels all
           remaining denominators). */
    }

    /* any remaining prime factor (greater than m) belongs entirely to the
       numerator; multiply it in. */
    if (factor_val > 1) {
        result *= factor_val;
    }

    return result;
}
