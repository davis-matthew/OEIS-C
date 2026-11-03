/*
 * A173906  Total number of digits of the squares of all n-digit numbers.
 * https://oeis.org/A173906
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (9/5)*n*10^n - ceil((sqrt(10)-1)*10^(n-1)), a(1)=16
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Validated: partial, pass, 20 terms through n = 20 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses the closed‑form expression derived in the OEIS entry.
   It requires the functions pow10(idx_t) and mul_small(seq_t, unsigned int)
   provided by the utils library. No floating‑point or heavy computation is
   used, so it works for every supported seq_t type. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

static seq_t floor_sqrt10_times_pow10(idx_t n)
{
    seq_t result   = 0U;
    seq_t remainder = 0U;

    for (idx_t i = 0; i < n; ++i) {
        /* bring down the next two decimal digits: multiply by 100 */
        remainder = mul_small(remainder, 100U);
        if (i == 0)                /* the first group is "10" */
            remainder += 10U;

        /* 20*current_root */
        seq_t base = mul_small(result, 20U);

        /* find the largest digit x such that (base + x)*x <= remainder */
        int x;
        for (x = 9; x >= 0; --x) {
            seq_t temp = base + (seq_t)x;
            temp = mul_small(temp, (unsigned int)x);
            if (temp <= remainder)
                break;
        }

        /* append the digit to the root */
        result = mul_small(result, 10U) + (seq_t)x;

        /* update the remainder */
        seq_t temp = base + (seq_t)x;
        temp = mul_small(temp, (unsigned int)x);
        remainder -= temp;
    }
    return result;     /* floor(sqrt(10) * 10^(n-1)) */
}

seq_t a(idx_t n)
{
    if (n == 1)
        return 16U;   /* special case for n = 1 */

    /* powers of 10 needed in the formula */
    seq_t pow10_n1 = pow10(n - 1);          /* 10^(n-1)      */
    seq_t pow10_n  = mul_small(pow10_n1, 10U); /* 10^n          */

    /* term1 = (9*n*10^n)/5 */
    seq_t numerator = pow10_n * ((seq_t)9 * (seq_t)n);
    seq_t term1     = numerator / 5U;          /* exact division */

    /* ceil((sqrt(10)-1)*10^(n-1)) = floor(sqrt(10)*10^(n-1)) + 1 - 10^(n-1) */
    seq_t floor_sqrt10 = floor_sqrt10_times_pow10(n);
    seq_t ceil_term    = floor_sqrt10 + 1U - pow10_n1;

    return term1 - ceil_term;
}
