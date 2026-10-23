/*
 * A101698  Numbers n such that n = d_1*d_1!+d_2*d_2!+...+d_k*d_k!, where d_1
 *          d_2 ... d_k is the decimal expansion of n.
 * https://oeis.org/A101698
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function finds all integers n such that
   n = Σ d_i * d_i! where d_i are the decimal digits of n.
   It returns the n-th smallest such integer (a(1)=0). */

#include <stdlib.h>
#include <stdbool.h>

/* Precomputed factorials up to 9! (as seq_t to avoid overflow). */
static seq_t fact[10];

/* Contributions d * d! for digits 0..9. */
static seq_t contrib[10];

/* Array of found numbers (ascending order). */
static seq_t *values = NULL;
static idx_t value_count = 0;
static idx_t value_capacity = 0;
static bool precomputed = false;

/* Compute factorials and contributions once. */
static void init_constants(void)
{
    fact[0] = (seq_t)1;
    for (idx_t i = 1; i <= 9; ++i)
        fact[i] = fact[i-1] * (seq_t)i;

    for (idx_t d = 0; d <= 9; ++d)
        contrib[d] = (seq_t)d * fact[d];
}

/* Append a number to the dynamic array. */
static void append_value(seq_t v)
{
    if (value_count == value_capacity) {
        value_capacity = value_capacity ? value_capacity * 2 : 1024;
        values = (seq_t *)realloc(values, value_capacity * sizeof(seq_t));
    }
    values[value_count++] = v;
}

/* Populate all values up to the theoretical maximum. */
static void precompute(void)
{
    if (precomputed) return;
    init_constants();

    /* Max n satisfies n <= 8 * 9 * 9! = 26,127,360. */
    const idx_t MAX_N = 26127360u;

    for (idx_t num = 0; num <= MAX_N; ++num) {
        seq_t sum = (seq_t)0;
        idx_t tmp = num;
        /* Special case for 0: sum stays 0. */
        if (tmp == 0)
            sum = contrib[0];
        else {
            while (tmp > 0) {
                idx_t d = tmp % 10;
                sum += contrib[d];
                tmp /= 10;
            }
        }
        if (sum == (seq_t)num)
            append_value((seq_t)num);
    }
    precomputed = true;
}

/* OEIS function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* Undefined for indices < 1 */
    precompute();
    if (n <= value_count)
        return values[n - 1];
    /* For indices beyond the list, return 0 (or any sentinel). */
    return (seq_t)0;
}
