/*
 * A110560  Numerators of T(n+1)/n! reduced to lowest terms, where T(n) are
 *          the triangular numbers A000217.
 * https://oeis.org/A110560
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, frac, nonn
 * Formula:   a(n) = reduced numerator of (n+1)(n+2)/(2*n!)
 * Range:     u8 n <= 14, u16 n <= 1029, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   exp_in_factorial, process_val, seq_pow_loop
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/exp_in_factorial.h"
#include "../utils/process_val.h"
#include "../utils/seq_pow_loop.h"

seq_t a(idx_t n)
{
    /* N = (n+1)*(n+2) */
    seq_t num = ((seq_t)(n + 1)) * ((seq_t)(n + 2));

    /* factor (n+1) */
    {
        idx_t tmp = n + 1;
        for (idx_t p = 2; p * p <= tmp; ++p) {
            if (tmp % p == 0) {
                idx_t e = 0;
                while (tmp % p == 0) {
                    ++e;
                    tmp /= p;
                }
                idx_t cancel = e;
                idx_t v = exp_in_factorial(n, p);
                if (p == 2) v += 1;
                if (cancel > v) cancel = v;
                seq_t div = seq_pow_loop((seq_t)p, cancel);
                num /= div;
            }
        }
        if (tmp > 1) {
            idx_t p = tmp;
            idx_t cancel = 1;
            idx_t v = exp_in_factorial(n, p);
            if (p == 2) v += 1;
            if (cancel > v) cancel = v;
            seq_t div = seq_pow_loop((seq_t)p, cancel);
            num /= div;
        }
    }

    /* factor (n+2) */
    {
        idx_t tmp = n + 2;
        for (idx_t p = 2; p * p <= tmp; ++p) {
            if (tmp % p == 0) {
                idx_t e = 0;
                while (tmp % p == 0) {
                    ++e;
                    tmp /= p;
                }
                idx_t cancel = e;
                idx_t v = exp_in_factorial(n, p);
                if (p == 2) v += 1;
                if (cancel > v) cancel = v;
                seq_t div = seq_pow_loop((seq_t)p, cancel);
                num /= div;
            }
        }
        if (tmp > 1) {
            idx_t p = tmp;
            idx_t cancel = 1;
            idx_t v = exp_in_factorial(n, p);
            if (p == 2) v += 1;
            if (cancel > v) cancel = v;
            seq_t div = seq_pow_loop((seq_t)p, cancel);
            num /= div;
        }
    }

    return num;
}
