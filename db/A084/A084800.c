/*
 * A084800  Least positive integers, all distinct, that satisfy Sum_{n>0}
 *          1/a(n)^z = 0, where z=(4+I*3)/5.
 * https://oeis.org/A084800
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence:
 * a(n) = the smallest positive integer not previously chosen such that
 * |S_{n-1} + 1/m^z| < |S_{n-1}|,
 * where z = (4 + 3i)/5 and S_{k} = ∑_{i=1}^{k} 1/a(i)^z.
 * a(1) = 1. */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* ---------- private state ------------------------------------------ */
static size_t chosen_cap = 0;          /* capacity of chosen array   */
static size_t chosen_len = 0;          /* number of terms generated  */
static seq_t *chosen = NULL;          /* dynamic array of terms     */

static size_t used_cap = 0;            /* capacity of used array     */
static bool *used = NULL;             /* boolean array of used ints */

static double sum_r = 0.0;            /* current real part of sum   */
static double sum_i = 0.0;            /* current imaginary part    */

static unsigned long long next_candidate = 2; /* next integer to test */

/* Ensure that 'used' can index 'need'  (indices start at 1). */
static void ensure_used_capacity(size_t need)
{
    if (need <= used_cap)
        return;
    size_t new_cap = used_cap ? used_cap << 1 : 64;
    while (new_cap < need)
        new_cap <<= 1;
    bool *new_arr = (bool *)calloc(new_cap, sizeof(bool));
    if (!new_arr)
        abort();
    if (used) {
        memcpy(new_arr, used, used_cap * sizeof(bool));
        free(used);
    }
    used = new_arr;
    used_cap = new_cap;
}

/* Ensure that 'chosen' can store 'need' elements. */
static void ensure_chosen_capacity(size_t need)
{
    if (need <= chosen_cap)
        return;
    size_t new_cap = chosen_cap ? chosen_cap << 1 : 64;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *new_arr = (seq_t *)realloc(chosen, new_cap * sizeof(seq_t));
    if (!new_arr)
        abort();
    chosen = new_arr;
    chosen_cap = new_cap;
}

/* Add the next term of the sequence. */
static void add_next(void)
{
    double old_sq = sum_r * sum_r + sum_i * sum_i;
    for (;;) {
        /* Expand used array if needed. */
        ensure_used_capacity((size_t)next_candidate + 1);
        if (used[next_candidate]) {
            ++next_candidate;
            continue;
        }
        unsigned long long m = next_candidate;
        /* compute m^{-z} with z = 0.8 + 0.6i  == (4+3i)/5 */
        double ln_m = log((double)m);          /* ln(m)            */
        double mag   = exp(-0.8 * ln_m);       /* |m^{-z}|         */
        double ang   = -0.6 * ln_m;            /* argument of m^{-z} */
        double term_r = mag * cos(ang);
        double term_i = mag * sin(ang);
        double new_r  = sum_r + term_r;
        double new_i  = sum_i + term_i;
        double new_sq = new_r * new_r + new_i * new_i;
        if (new_sq < old_sq) {
            /* Accept m as the next term. */
            used[next_candidate] = true;
            sum_r = new_r;
            sum_i = new_i;
            ++next_candidate;
            /* Store the new term. */
            ensure_chosen_capacity(chosen_len + 1);
            chosen[chosen_len++] = (seq_t)m;
            return;
        }
        ++next_candidate;
    }
}

/* Public entry point. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0; /* undefined for 0th element, return 0 */
    if (chosen_len == 0) {
        /* initialise first term a(1) = 1 */
        sum_r = 1.0;
        sum_i = 0.0;
        ensure_chosen_capacity(1);
        chosen[0] = (seq_t)1;
        chosen_len = 1;
        ensure_used_capacity(2);
        used[1] = true;          /* mark 1 as used */
        next_candidate = 2;
    }
    while (chosen_len < (size_t)n)
        add_next();               /* generate until we reach 'n' */
    return chosen[n - 1];
}
