/*
 * A205356  Number of (n+1) X 4 0..2 arrays with every 2 X 2 subblock having
 *          the same number of equal diagonal or antidiagonal elements, and
 *          new values 0..2 introduced in row major order.
 * https://oeis.org/A205356
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 23*a(n-1) - 187*a(n-2) + 645*a(n-3) - 912*a(n-4) +
 *            432*a(n-5) Initial values: a(1)=194, a(2)=1562, a(3)=12866,
 *            a(4)=107330, a(5)=904514
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
#include <vector>
#endif

/* ----------------- static storage ------------------------------------- */
static const seq_t init_vals[6] = {0, 194, 1562, 12866, 107330, 904514};

#ifndef __cplusplus
/* C implementation uses a raw array */
static seq_t *seq_arr = NULL;
static size_t seq_arr_size = 0;
static inline seq_t get_seq(idx_t i) {
    return seq_arr[i];
}
static inline void set_seq(idx_t i, seq_t v) {
    seq_arr[i] = v;
}
#else
/* C++ implementation uses std::vector for proper construction of big ints */
static std::vector<seq_t> seq_vec;
static inline seq_t get_seq(idx_t i) {
    return seq_vec[i];
}
static inline void set_seq(idx_t i, seq_t v) {
    seq_vec[i] = v;
}
#endif

static idx_t computed_upto = 0;

static void ensure_capacity(idx_t n) {
    if (n < 1) return;
    size_t needed = (size_t)n + 1; /* +1 because we ignore index 0 */
#ifdef __cplusplus
    if (seq_vec.size() < needed) seq_vec.resize(needed);
#else
    if (seq_arr_size < needed) {
        size_t new_size = seq_arr_size ? seq_arr_size * 2 : needed;
        if (new_size < needed) new_size = needed;
        seq_arr = (seq_t *)realloc(seq_arr, new_size * sizeof(seq_t));
        seq_arr_size = new_size;
    }
#endif
}

/* ----------------- main function -------------------------------------- */
seq_t a(idx_t n) {
    if (n <= 5) return init_vals[n];

    /* ensure storage for indices up to n */
    ensure_capacity(n);

    /* initialise base values if not done yet */
    if (computed_upto < 5) {
        for (idx_t i = 1; i <= 5; ++i) {
            set_seq(i, init_vals[i]);
        }
        computed_upto = 5;
    }

    /* compute sequence up to n */
    for (idx_t i = computed_upto + 1; i <= n; ++i) {
        seq_t pos = get_seq(i - 1) * (seq_t)23
                  + get_seq(i - 3) * (seq_t)645
                  + get_seq(i - 5) * (seq_t)432;
        seq_t neg = get_seq(i - 2) * (seq_t)187
                  + get_seq(i - 4) * (seq_t)912;
        set_seq(i, pos - neg);
    }
    computed_upto = n;

    return get_seq(n);
}
