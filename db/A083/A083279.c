/*
 * A083279  Length of odd/even runs in A061419, where a(2n)=length of n-th run
 *          of even numbers in A061419 and a(2n-1)=length of n-th run of odd
 *          numbers.
 * https://oeis.org/A083279
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   let b(1) = 1, b(k) = ceil(3*b(k-1)/2) (A061419); a(2n-1) and
 *            a(2n) are the lengths of the n-th run of odd and the n-th run of
 *            even terms of b. Computed by direct simulation with a small
 *            bignum
 * Range:     u8 n <= 93, u16 n <= 93, u32 n <= 93, u64 n <= 93, u128 n <= 93,
 *            big any n
 * Validated: partial, pass, 93 terms through n = 93 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint32_t *bn_x = NULL;
static size_t bn_len = 0, bn_cap = 0;
static uint32_t *runs_arr = NULL;
static size_t runs_n = 0, runs_cap = 0;
static uint32_t run_len = 0;
static unsigned cur_par = 1;

static void bn_step(void)
{
    uint64_t carry = bn_x[0] & 1u;
    for (size_t i = 0; i < bn_len; i++) {
        uint32_t xi = bn_x[i];
        uint32_t hi = (i + 1 < bn_len) ? bn_x[i + 1] : 0u;
        uint32_t y = (xi >> 1) | (hi << 31);
        uint64_t s = (uint64_t)xi + (uint64_t)y + carry;
        bn_x[i] = (uint32_t)s;
        carry = s >> 32;
    }
    if (carry) {
        if (bn_len == bn_cap) {
            bn_cap *= 2;
            bn_x = (uint32_t *)realloc(bn_x, bn_cap * sizeof(uint32_t));
            if (!bn_x) abort();
        }
        bn_x[bn_len++] = (uint32_t)carry;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (!bn_x) {
        bn_cap = 16;
        bn_x = (uint32_t *)malloc(bn_cap * sizeof(uint32_t));
        if (!bn_x) abort();
        bn_x[0] = 1;
        bn_len = 1;
        run_len = 1;
        cur_par = 1;
        runs_cap = 64;
        runs_arr = (uint32_t *)malloc(runs_cap * sizeof(uint32_t));
        if (!runs_arr) abort();
        runs_n = 0;
    }
    while (runs_n < (size_t)n) {
        bn_step();
        unsigned p = bn_x[0] & 1u;
        if (p == cur_par) {
            run_len++;
        } else {
            if (runs_n == runs_cap) {
                runs_cap *= 2;
                runs_arr = (uint32_t *)realloc(runs_arr, runs_cap * sizeof(uint32_t));
                if (!runs_arr) abort();
            }
            runs_arr[runs_n++] = run_len;
            run_len = 1;
            cur_par = p;
        }
    }
    return (seq_t)runs_arr[n - 1];
}
