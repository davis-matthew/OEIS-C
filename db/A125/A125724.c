/*
 * A125724  If "sumdigit" denotes the sum of the digits of a number then these
 *          are the numbers n such that n=sumdigit(sumdigit(n)^sumdigit(n)).
 * https://oeis.org/A125724
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, fini, full, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Validated: full, pass, 7 terms through n = 7 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = nth integer n such that
   n = sumdigit( sumdigit(n)^sumdigit(n) )
   (OEIS A125724) */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* ---------- big‑integer helper: multiply decimal number by a small int ---------- */
static uint64_t sumdigit_pow_uint(unsigned s)
{
    /* calculate s^s using a decimal array, then sum its digits */
    static uint8_t digits[1200];   /* enough for 200^200 (~460 digits) */
    size_t len = 1;
    digits[0] = 1;                 /* start with 1 */

    for (unsigned i = 0; i < s; ++i) {
        uint32_t carry = 0;
        for (size_t j = 0; j < len; ++j) {
            uint32_t prod = (uint32_t)digits[j] * s + carry;
            digits[j] = (uint8_t)(prod % 10);
            carry   = prod / 10;
        }
        while (carry) {
            digits[len++] = (uint8_t)(carry % 10);
            carry /= 10;
        }
    }

    /* sum digits of the result */
    uint64_t sum = 0;
    for (size_t j = 0; j < len; ++j)
        sum += digits[j];
    return sum;
}

/* ---------- solution storage ---------- */
#define MAX_SOL 512
static seq_t sol[MAX_SOL];
static size_t sol_len = 0;

/* comparison for qsort (unsigned) */
static int cmp_seq(const void *a, const void *b)
{
    seq_t x = *(const seq_t *)a;
    seq_t y = *(const seq_t *)b;
    return (x < y) ? -1 : (x > y);
}

/* ---------- generate all solutions up to s = 200 ---------- */
static void compute_solutions(void)
{
    static bool done = false;
    if (done) return;
    const unsigned S_MAX = 200;          /* sufficient upper bound for s */

    for (unsigned s = 1; s <= S_MAX; ++s) {
        uint64_t k = sumdigit_pow_uint(s);       /* k = sumdigit(s^s) */
        /* sum of digits of k */
        uint64_t sumk = 0;
        uint64_t t = k;
        while (t) { sumk += t % 10; t /= 10; }
        if (sumk == s) {
            if (sol_len < MAX_SOL)
                sol[sol_len++] = (seq_t)k;      /* store solution */
        }
    }

    /* sort solutions */
    if (sol_len > 1)
        mem_qsort(sol, sol_len, sizeof(seq_t), cmp_seq);

    done = true;
}

/* ---------- public function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* undefined before offset 1 */

    compute_solutions();
    if ((size_t)n > sol_len) return (seq_t)0;   /* out of bounds, should not happen */
    return sol[n-1];
}
