/*
 * A124176  Consider the map f that sends m to m + (sum of odd digits of m) -
 *          (sum of even digits of m). Sequence gives numbers m such that
 *          f^(k)(m) = m for some k.
 * https://oeis.org/A124176
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 172, u32 n <= 172, u64 n <= 172,
 *            u128 n <= 172, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence lists integers m for which repeatedly applying
   f(m) = m + (sum of odd digits of m) – (sum of even digits of m)
   eventually returns to m (i.e., m lies on a cycle of the map f). */

#include <stdlib.h>
#include <stdbool.h>

/* Compute one step of the map f */
static inline seq_t f(seq_t x)
{
    seq_t sumOdd = 0, sumEven = 0;
    seq_t y = x;
    while (y > 0) {
        seq_t d = y % 10;
        if (d & 1)
            sumOdd += d;
        else
            sumEven += d;
        y /= 10;
    }
    int diff = (int)sumOdd - (int)sumEven;           /* difference fits in int */
    if (diff >= 0)
        return x + (seq_t)diff;
    else
        return x - (seq_t)(-diff);                 /* subtract positive amount */
}

/* Test whether a given integer m (0‑based) lies on a cycle of f. */
static bool is_cycle_node(idx_t m)
{
    seq_t start = (seq_t)m;

    /* Floyd's cycle‑finding algorithm */
    seq_t tortoise = f(start);
    seq_t hare = f(f(start));
    while (tortoise != hare) {
        tortoise = f(tortoise);
        hare = f(f(hare));
    }

    /* Find start of cycle (mu). */
    seq_t mu = start;
    while (mu != tortoise) {
        mu = f(mu);
        tortoise = f(tortoise);
    }

    /* Compute cycle length (lambda). */
    idx_t lambda = 1;
    seq_t cur = f(mu);
    while (cur != mu) {
        cur = f(cur);
        ++lambda;
    }

    /* Check if the starting value is inside the cycle. */
    cur = mu;
    for (idx_t i = 0; i < lambda; ++i) {
        if (cur == start)
            return true;
        cur = f(cur);
    }
    return false;
}

/* Cache for the sequence. */
static seq_t *seq_cache = NULL;
static size_t seq_len   = 0;   /* number of terms stored */
static size_t seq_cap   = 0;   /* current capacity of seq_cache */
static idx_t next_m     = 0;   /* next integer to test */

/* Ensure the cache has space for at least one more term. */
static void ensure_capacity(void)
{
    if (seq_len >= seq_cap) {
        size_t new_cap = seq_cap ? seq_cap << 1 : 64;
        seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        seq_cache = tmp;
        seq_cap   = new_cap;
    }
}

/* Main function: returns the n-th term (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0)    /* sequence is offset 1; return 0 for safety */
        return (seq_t)0;

    while (seq_len < n) {
        if (is_cycle_node(next_m)) {
            ensure_capacity();
            seq_cache[seq_len++] = (seq_t)next_m;
        }
        ++next_m;
    }
    return seq_cache[n - 1];
}
