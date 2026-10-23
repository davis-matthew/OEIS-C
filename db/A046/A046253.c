/*
 * A046253  Equal to the sum of its nonzero digits raised to its own power.
 * https://oeis.org/A046253
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: enumerates all fixed points of the function that maps a number to the
 * sum of its non‑zero digits each raised to themselves.  The sequence is finite
 * and contains 0, 1, 3435, 438579088.  It is found by exhaustive search over
 * digit‑count combinations (up to 10 digits). */
#include <stddef.h>
#include <stdbool.h>

/* Powers d^d for d = 1..9 */
static const seq_t POW[10] = {
    0ULL,       /* unused */
    1ULL,       /* 1^1   */
    4ULL,       /* 2^2   */
    27ULL,      /* 3^3   */
    256ULL,     /* 4^4   */
    3125ULL,    /* 5^5   */
    46656ULL,   /* 6^6   */
    823543ULL,  /* 7^7   */
    16777216ULL,/* 8^8   */
    387420489ULL/* 9^9   */
};

/* Upper bound for candidate numbers (all lie below 10^10). */
static const seq_t LIMIT = (seq_t)10000000000ULL;

/* Storage for discovered terms. */
static seq_t solutions[64];
static size_t sol_count = 0;

/* Recursive enumeration of digit‑count vectors. */
static void backtrack(int d, int remaining, seq_t sum, int cnt[10])
{
    if (d == 10) {
        if (sum >= LIMIT) return;

        /* Count digits of sum. */
        unsigned char digitCnt[10] = {0};
        seq_t tmp = sum;
        if (tmp == 0) {
            digitCnt[0] = 1;
        } else {
            while (tmp) {
                digitCnt[(int)(tmp % 10)]++;
                tmp /= 10;
            }
        }

        /* Check that non‑zero digits match the chosen counts. */
        for (int dig = 1; dig <= 9; ++dig) {
            if (digitCnt[dig] != cnt[dig]) return;
        }

        /* Avoid duplicates. */
        for (size_t i = 0; i < sol_count; ++i) {
            if (solutions[i] == sum) return;
        }

        solutions[sol_count++] = sum;
        return;
    }

    int maxc = remaining;
    for (int c = 0; c <= maxc; ++c) {
        seq_t inc = (seq_t)c * POW[d];
        seq_t newSum = sum + inc;
        if (newSum >= LIMIT) break;   /* larger c will only increase newSum */
        cnt[d] = c;
        backtrack(d + 1, remaining - c, newSum, cnt);
    }
}

/* Simple insertion sort of solutions. */
static void sort_solutions(void)
{
    for (size_t i = 1; i < sol_count; ++i) {
        seq_t val = solutions[i];
        size_t j = i;
        while (j > 0 && solutions[j - 1] > val) {
            solutions[j] = solutions[j - 1];
            --j;
        }
        solutions[j] = val;
    }
}

seq_t a(idx_t n)
{
    static bool initialized = false;
    if (!initialized) {
        int cnt[10] = {0};
        backtrack(1, 10, (seq_t)0, cnt);
        sort_solutions();
        initialized = true;
    }

    if (n == 0) return 0;                     /* offset starts at 1 */
    if ((size_t)n <= sol_count) {
        return solutions[(size_t)(n - 1)];
    }
    return 0;                                 /* no further terms */
}
