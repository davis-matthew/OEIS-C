/*
 * A129913  a(n) is the number of ways of producing n, with an expression
 *          like: +- 1 # 2 # 3 # 4 # 5 # 6 # 7 # 8 # 9, where each # is
 *          replaced by any one of +,-,*,/; and the value is computed going
 *          left to right.
 * https://oeis.org/A129913
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   ll_gcd, reduce
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = number of ways to produce n using
 * ±1 # 2 # 3 # 4 # 5 # 6 # 7 # 8 # 9,
 * where # is one of +,-,*,/ evaluated strictly left‑to‑right.
 * The function precomputes all 2·4⁸ possibilities and stores the
 * counts up to the largest attainable value (9! = 362880).
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/ll_gcd.h"
#include "../utils/reduce.h"

#define MAX_VAL 362880   /* 1*2*3*4*5*6*7*8*9 */

/* Apply one operator to the current rational value.
 * op == 0:+, 1:-, 2:*, 3:/, operand: integer i (2…9) */
static void apply_op(long long *num, long long *den, int op, int operand)
{
    switch (op) {
        case 0:  /* + */
            *num = *num + (long long)operand * (*den);
            break;
        case 1:  /* - */
            *num = *num - (long long)operand * (*den);
            break;
        case 2:  /* * */
            *num = *num * (long long)operand;
            break;
        case 3:  /* / */
            *den = *den * (long long)operand;
            break;
    }
    reduce(num, den);
}

/* Build the count table once.  The table is indexed by the
 * integer result of the expression (0…MAX_VAL). */
static void init_counts(seq_t *counts)
{
    for (int mask = 0; mask < (1 << 16); ++mask) {   /* 4⁸ operator choices */
        for (int init = 0; init < 2; ++init) {       /* first sign + or - */
            long long num = init ? - (long long)1 : (long long)1;
            long long den = 1;
            int bits = mask;
            for (int operand = 2; operand <= 9; ++operand) {
                int op = bits & 3;   /* two bits per operator */
                bits >>= 2;
                apply_op(&num, &den, op, operand);
            }
            if (den == 1 && num >= 0 && num <= MAX_VAL) {
                counts[(size_t)num] += 1U;
            }
        }
    }
}

/* SEQUENCE: a(n) */
seq_t a(idx_t n)
{
    static seq_t *counts = NULL;
    static bool initialized = false;

    if (!initialized) {
        size_t sz = (size_t)MAX_VAL + 1;
        counts = (seq_t *)calloc(sz, sizeof(seq_t));
        if (!counts) return 0;     /* allocation failure safeguard */
        init_counts(counts);
        initialized = true;
    }

    if (n > (idx_t)MAX_VAL) return 0;
    return counts[(size_t)n];
}
