/*
 * A171827  Nonnegative integers that can be made by using exactly four fours
 *          (4 4's) and the four basic operators {+, -, *, /}.
 * https://oeis.org/A171827
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 26, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes all non‑negative integers obtainable with exactly four 4's
 * using the operators +, -, *, /.  The function returns the n‑th term
 * of the sorted list of such integers (a(1) is the smallest value).  */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#define O_MAX 256   /* maximum reachable value (4*4*4*4) */

/* apply operator op to x and y.
 * op: 0 -> +, 1 -> -, 2 -> *, 3 -> /
 * returns true if the operation is valid (division test included) */
static inline bool apply_op(long long x, long long y, int op, long long *res)
{
    switch (op) {
        case 0: *res = x + y; return true;
        case 1: *res = x - y; return true;
        case 2: *res = x * y; return true;
        case 3: return (y != 0 && x % y == 0) ? (*res = x / y, true) : false;
    }
    return false;   /* unreachable */
}

/* build the sorted sequence of reachable values once */
static void build_sequence(seq_t *seq_out, idx_t *seq_len)
{
    bool present[O_MAX + 1] = { false };
    long long t1, t2, t3, t4, t5, t6, tfinal;
    int op1, op2, op3, op4;

    /* Enumerate all 5 binary tree shapes with 3 operator positions. */
    for (op1 = 0; op1 < 4; ++op1)
    for (op2 = 0; op2 < 4; ++op2)
    for (op3 = 0; op3 < 4; ++op3) {
        /* Shape 1: ((4 op1 4) op2 4) op3 4 */
        if (!apply_op(4, 4, op1, &t1)) goto skip1;
        if (!apply_op(t1, 4, op2, &t2)) goto skip1;
        if (!apply_op(t2, 4, op3, &t3)) goto skip1;
        if (t3 >= 0 && t3 <= O_MAX) present[t3] = true;
    skip1: ;

        /* Shape 2: (4 op1 (4 op2 4)) op3 4 */
        if (!apply_op(4, 4, op2, &t1)) goto skip2;
        if (!apply_op(4, t1, op1, &t2)) goto skip2;
        if (!apply_op(t2, 4, op3, &t3)) goto skip2;
        if (t3 >= 0 && t3 <= O_MAX) present[t3] = true;
    skip2: ;

        /* Shape 3: 4 op1 ((4 op2 4) op3 4) */
        if (!apply_op(4, 4, op2, &t1)) goto skip3;
        if (!apply_op(t1, 4, op3, &t2)) goto skip3;
        if (!apply_op(4, t2, op1, &t3)) goto skip3;
        if (t3 >= 0 && t3 <= O_MAX) present[t3] = true;
    skip3: ;

        /* Shape 4: 4 op1 (4 op2 (4 op3 4)) */
        if (!apply_op(4, 4, op3, &t1)) goto skip4;
        if (!apply_op(4, t1, op2, &t2)) goto skip4;
        if (!apply_op(4, t2, op1, &t3)) goto skip4;
        if (t3 >= 0 && t3 <= O_MAX) present[t3] = true;
    skip4: ;

        /* Shape 5: (4 op1 4) op3 (4 op2 4) */
        if (!apply_op(4, 4, op1, &t1)) goto skip5;
        if (!apply_op(4, 4, op2, &t2)) goto skip5;
        if (!apply_op(t1, t2, op3, &t3)) goto skip5;
        if (t3 >= 0 && t3 <= O_MAX) present[t3] = true;
    skip5: ;
    }

    /* Transfer the present flags into the sorted sequence array. */
    idx_t cnt = 0;
    for (int i = 0; i <= O_MAX; ++i)
        if (present[i]) {
            seq_out[cnt++] = (seq_t)i;
        }
    *seq_len = cnt;
}

/* sequence element retrieval */
seq_t a(idx_t n)
{
    static bool initialized = false;
    static seq_t seq[O_MAX + 1];
    static idx_t seq_len = 0;

    if (!initialized) {
        build_sequence(seq, &seq_len);
        initialized = true;
    }
    if (n == 0 || n > seq_len) return (seq_t)0;
    return seq[n - 1];
}
