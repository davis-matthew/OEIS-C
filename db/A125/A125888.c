/*
 * A125888  Numerator of sum of first n ratios of Fibonacci to Lucas numbers.
 * https://oeis.org/A125888
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, frac
 * Formula:   a(n) = numerator(SUM_{i=1..n} F(i)/L(i))
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* F(i) = Fibonacci number, L(i) = Lucas number.  The fraction is reduced after each term. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"

#define MAX_ARRAY_SIZE 50001   /* enough for n up to ~10000 */

static seq_t a_vals[MAX_ARRAY_SIZE];
static idx_t current_n = 0;
static seq_t cur_num = 0;      /* numerator of the cumulative sum */
static seq_t cur_den = 1;      /* denominator   of the cumulative sum */

/* Fibonacci recurrence: F(0)=0, F(1)=1 */
static seq_t fib_prev = 0;     /* F(k-1) when current_n = k-1  */
static seq_t fib_curr = 1;     /* F(k)   when current_n = k-1  */

/* Lucas recurrence: L(0)=2, L(1)=1 */
static seq_t luc_prev = 2;     /* L(k-1) when current_n = k-1  */
static seq_t luc_curr = 1;     /* L(k)   when current_n = k-1  */

static bool initialized = false;

/* Ensure the first term (n=0) is initialized */
static void init_sequence(void) {
    if (!initialized) {
        a_vals[0] = 0;
        current_n = 0;
        cur_num   = 0;
        cur_den   = 1;
        fib_prev  = 0;
        fib_curr  = 1;
        luc_prev  = 2;
        luc_curr  = 1;
        initialized = true;
    }
}

seq_t a(idx_t n) {
    init_sequence();

    /* Guard against requests beyond the pre‑allocated array */
    if (n >= MAX_ARRAY_SIZE) {
        /* In the unlikely event of a larger n, we could extend the array, */
        /* but for the intended use (n ≤ ~10 000) this is sufficient. */
        return 0;
    }

    /* If the requested term is already computed, return it immediately */
    if (n <= current_n) {
        return a_vals[n];
    }

    /* Compute terms up to n incrementally */
    for (idx_t i = current_n + 1; i <= n; ++i) {
        /* Current Fibonacci and Lucas numbers for this i */
        seq_t Fi = fib_curr;  /* F(i)   */
        seq_t Li = luc_curr;  /* L(i)   */

        /* Add the fraction Fi/Li to the cumulative sum */
        seq_t new_num = cur_num * Li + cur_den * Fi;
        seq_t new_den = cur_den * Li;
        seq_t g = gcd(new_num, new_den);
        new_num /= g;
        new_den /= g;
        cur_num = new_num;
        cur_den = new_den;

        /* Store the numerator as the sequence value */
        a_vals[i] = cur_num;

        /* Prepare Fibonacci and Lucas values for the next index */
        seq_t next_fib = fib_prev + fib_curr;
        seq_t next_luc = luc_prev + luc_curr;
        fib_prev = fib_curr;
        fib_curr = next_fib;
        luc_prev = luc_curr;
        luc_curr = next_luc;
    }

    current_n = n;
    return a_vals[n];
}
