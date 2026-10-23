/*
 * A105047  Form an addition table of the primes; a(n) is the number of even
 *          numbers that appear for the first time in column n.
 * https://oeis.org/A105047
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) is the number of even sums that appear for the first time
 * in the column corresponding to the n-th prime when building the addition
 * table of all primes.  The table entry in column n is p_n + p_j for
 * 1 ≤ j ≤ n.  As each new column is added we keep a set of the even sums
 * already seen; a(n) counts the new ones that arise in column n.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/get_prime.h"

/* ---------- Dynamic arrays ----------
 * Prime list (idx_t)                                            */
static idx_t *prime_list = NULL;
static size_t prime_cnt = 0;   /* number of primes stored */
static size_t prime_cap = 0;   /* allocated capacity */

/* ---------- a(n) memoisation ----------
 * a_arr[i] holds a(i) for i≥1; a_arr[0] is unused dummy      */
static seq_t *a_arr = NULL;
static size_t a_cnt = 0;       /* next free index; a_arr[a_cnt-1] last filled */
static size_t a_cap = 0;

/* ---------- Bitset of even sums ----------
 * seen_arr[s] == 1 iff even sum s has appeared      */
static uint8_t *seen_arr = NULL;
static size_t seen_cap = 0;   /* allocated capacity */

/* Make sure prime_list can hold at least min_cap primes */
static void ensure_prime_cap(size_t min_cap) {
    if (prime_cap < min_cap) {
        size_t new_cap = prime_cap ? prime_cap * 2 : 16;
        if (new_cap < min_cap) new_cap = min_cap;
        idx_t *tmp = (idx_t *)realloc(prime_list, new_cap * sizeof(idx_t));
        if (!tmp) exit(1);
        prime_list = tmp;
        prime_cap = new_cap;
    }
}

/* Make sure a_arr can hold at least min_cap entries */
static void ensure_a_cap(size_t min_cap) {
    if (a_cap < min_cap) {
        size_t new_cap = a_cap ? a_cap * 2 : 16;
        if (new_cap < min_cap) new_cap = min_cap;
        seq_t *tmp = (seq_t *)realloc(a_arr, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        a_arr = tmp;
        a_cap = new_cap;
    }
}

/* Make sure seen_arr can index up to min_cap-1 */
static void ensure_seen_cap_func(size_t min_cap) {
    if (seen_cap < min_cap) {
        size_t old_cap = seen_cap;
        size_t new_cap = min_cap;
        uint8_t *tmp = (uint8_t *)realloc(seen_arr, new_cap * sizeof(uint8_t));
        if (!tmp) exit(1);
        seen_arr = tmp;
        if (new_cap > old_cap)
            memset(seen_arr + old_cap, 0, new_cap - old_cap);
        seen_cap = new_cap;
    }
}

seq_t a(idx_t n) {
    if (n < 1) return 0;

    /* Grow a_arr to have entries up to n */
    if (a_cnt <= (size_t)n) {
        if (a_cnt == 0) {               /* initialise first call */
            ensure_a_cap(1);
            a_arr[0] = 0;                /* a(0) unused */
            a_cnt = 1;
        }
        for (size_t cur = a_cnt; cur <= (size_t)n; ++cur) {
            /* Get next prime p_cur */
            idx_t next_prime_idx = (idx_t)(prime_cnt + 1);   /* 1‑based */
            seq_t prime_seq = get_prime(next_prime_idx);
            idx_t prime_val = (idx_t)prime_seq;
            ensure_prime_cap(prime_cnt + 1);
            prime_list[prime_cnt++] = prime_val;

            /* Ensure seen_arr can index up to 2*prime_val */
            size_t needed_seen = (size_t)(prime_val * 2) + 1;
            ensure_seen_cap_func(needed_seen);

            /* Count new even sums in this column */
            seq_t new_count = 0;
            for (size_t j = 0; j < prime_cnt; ++j) {
                idx_t sum = prime_val + prime_list[j];
                if ((sum & 1u) == 0) { /* even */
                    size_t idx = (size_t)sum;
                    if (!seen_arr[idx]) {
                        seen_arr[idx] = 1;
                        ++new_count;
                    }
                }
            }
            /* Store result for column cur (which is column number cur) */
            ensure_a_cap(a_cnt + 1);
            a_arr[a_cnt++] = new_count;
        }
    }
    return a_arr[n];
}
