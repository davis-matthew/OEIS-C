/*
 * A096306  Number of iterations of n -> n + (largest digit of n) needed for
 *          the trajectory of n to join the trajectory of A045844.
 * https://oeis.org/A096306
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = number of iterations of n -> n + (largest digit of n)
 *            needed for the trajectory of n to join the trajectory of A045844
 *            (the sequence starting with 1 and iterating n + largest digit of
 *            n)
 * Range:     u8 n <= 40, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Convert index to seq_t, but direct cast is fine because both are unsigned integral types. */
static seq_t largest_digit(seq_t v) {
    seq_t max = 0;
    while (v > 0) {
        seq_t d = v % 10;
        if (d > max) max = d;
        v /= 10;
    }
    return max;
}

/* Dynamic array for the trajectory of 1, built on demand. */
static seq_t *traj = NULL;
static size_t traj_size = 0, traj_cap = 0;

/* Ensure capacity for at least n elements. */
static void ensure_capacity(size_t n) {
    if (traj_size >= n) return;
    if (traj_cap == 0) traj_cap = 16;
    while (traj_cap < n) traj_cap <<= 1;
    traj = (seq_t *)realloc(traj, traj_cap * sizeof(seq_t));
}

/* Append a value to the trajectory array. */
static void append_traj(seq_t val) {
    ensure_capacity(traj_size + 1);
    traj[traj_size++] = val;
}

/* Extend trajectory until the last value is >= target. */
static void extend_traj_upto(seq_t target) {
    while (traj_size > 0 && traj[traj_size-1] < target) {
        seq_t last = traj[traj_size-1];
        seq_t next = last + largest_digit(last);
        append_traj(next);
    }
}

/* Binary search for value; returns 1 if found, else 0. */
static int have_in_traj(seq_t val) {
    size_t lo = 0, hi = traj_size;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (traj[mid] < val)
            lo = mid + 1;
        else
            hi = mid;
    }
    return (lo < traj_size && traj[lo] == val);
}

seq_t a(idx_t n) {
    if (n == 0) return 0;            /* offset starts at 1, values for n==0 are irrelevant */

    seq_t cur = (seq_t)n;

    /* Initialize trajectory of 1 once. */
    if (traj_size == 0) {
        append_traj((seq_t)1);
    }

    /* Ensure the trajectory covers the current value. */
    extend_traj_upto(cur);

    seq_t steps = 0;
    while (!have_in_traj(cur)) {
        cur += largest_digit(cur);
        steps++;
        /* extend trajectory if necessary for the new cur */
        extend_traj_upto(cur);
    }
    return steps;
}
