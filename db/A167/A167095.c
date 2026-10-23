/*
 * A167095  Number of reduced words of length n in Coxeter group on 42
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^13 = I.
 * https://oeis.org/A167095
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=42, a(2)=1722, a(3)=70602, a(4)=2894682,
 *            a(5)=118681962, a(6)=4865960442, a(7)=199504378122,
 *            a(8)=8179679503002, a(9)=335366859623082,
 *            a(10)=13750041244546362, a(11)=563751691026400842,
 *            a(12)=23113819332082434522, a(13)=947666592615379814541,
 *            a(n)=40·(a(n‑1)+…+a(n‑12)) – 820·a(n‑13)...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   seq_alloc_array, seq_free_array, seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/seq_alloc_array.h"
#include "../utils/seq_free_array.h"
#include "../utils/seq_from_str.h"

/* Cached values of the sequence */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* 13 initial terms (a(0) … a(12)) */
static seq_t init_vals[13];
static bool init_done = false;

/* Initialise the 13 known terms once */
static void init_initial_terms(void) {
    if (init_done) return;
    init_vals[0]  = seq_from_str("1");
    init_vals[1]  = seq_from_str("42");
    init_vals[2]  = seq_from_str("1722");
    init_vals[3]  = seq_from_str("70602");
    init_vals[4]  = seq_from_str("2894682");
    init_vals[5]  = seq_from_str("118681962");
    init_vals[6]  = seq_from_str("4865960442");
    init_vals[7]  = seq_from_str("199504378122");
    init_vals[8]  = seq_from_str("8179679503002");
    init_vals[9]  = seq_from_str("335366859623082");
    init_vals[10] = seq_from_str("13750041244546362");
    init_vals[11] = seq_from_str("563751691026400842");
    init_vals[12] = seq_from_str("23113819332082434522");
    init_done = true;
}

/* Ensure that the cache can hold at least new_len elements */
static void ensure_capacity(size_t new_len) {
    if (new_len <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < new_len) new_cap *= 2;
    seq_t *tmp = seq_alloc_array(new_cap);
    if (cache) {
        for (size_t i = 0; i < cache_len; ++i) tmp[i] = cache[i];
        seq_free_array(cache);
    }
    cache = tmp;
    cache_cap = new_cap;
}

/* Fill the cache up to (and including) index n */
static void compute_up_to(idx_t n) {
    size_t start = cache_len;
    size_t end   = (size_t)n + 1;        /* one past last needed index */
    if (end <= cache_len) return;

    ensure_capacity(end);

    for (size_t i = start; i < end; ++i) {
        if (i < 13) {                                   /* known terms */
            cache[i] = init_vals[(size_t)i];
        } else if (i == 13) {                           /* special case */
            seq_t sum = 0;
            for (size_t k = 1; k <= 12; ++k) sum += cache[i - k];
            seq_t val = 40 * sum;
            val -= 820 * cache[i - 13];
            val += 1;                                   /* constant term of numerator */
            cache[i] = val;
        } else {                                        /* n >= 14 */
            seq_t sum = 0;
            for (size_t k = 1; k <= 12; ++k) sum += cache[i - k];
            seq_t val = 40 * sum;
            val -= 820 * cache[i - 13];
            cache[i] = val;
        }
    }
    cache_len = end;
}

/* Public function returning a(n) */
seq_t a(idx_t n) {
    init_initial_terms();
    if ((size_t)n >= cache_len) compute_up_to(n);
    return cache[(size_t)n];
}
