/*
 * A028229  Call m Egyptian if we can partition m = x_1+x_2+...+x_k into
 *          positive integers x_i such that Sum_{i=1..k} 1/x_i = 1; sequence
 *          gives all non-Egyptian numbers.
 * https://oeis.org/A028229
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, nice
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 13, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Validated: full, pass, 13 terms through n = 13 (2026-11-03)
 * Helpers:   gcd, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A028229: non‑Egyptian numbers (numbers <78 that cannot be expressed 
   as a sum of reciprocals summing to 1).  The sequence is finite;  
   all such numbers are below 78. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd.h"
#include "../utils/mem.h"

/* Recursively test whether the remaining fraction num/den can be achieved
   by adding unit fractions with denominators not exceeding m - sum_d,
   while keeping the total sum of denominators equal to m. */
static bool egyptian_rec(seq_t num, seq_t den, seq_t sum_d, uint64_t m)
{
    if (num == 0)                    /* fraction reduced to 0 → 1 achieved */
        return sum_d == (seq_t)m;

    if (sum_d > (seq_t)m)            /* exceeded the target sum */
        return false;

    seq_t m_seq = (seq_t)m;
    seq_t min_d = (den + num - 1) / num;     /* ceil(den/num) */
    seq_t max_d = m_seq - sum_d;             /* remaining denominator budget */
    if (min_d > max_d)
        return false;

    for (seq_t d = min_d; d <= max_d; ++d) {
        seq_t new_num = num * d - den;
        seq_t new_den = den * d;

        if (new_num == 0) {        /* exact match if denominator sums to m */
            if (sum_d + d == m_seq)
                return true;
            continue;
        }

        seq_t g = gcd(new_num, new_den);
        new_num /= g;
        new_den /= g;

        if (egyptian_rec(new_num, new_den, sum_d + d, m))
            return true;
    }
    return false;
}

/* True iff m has an Egyptian partition. */
static bool is_egyptian(uint64_t m)
{
    if (m == 1)
        return true;                /* 1 = 1/1 */
    return egyptian_rec((seq_t)1, (seq_t)1, (seq_t)0, m);
}

/* Euclidean algorithm for seq_t */

/* Return the nth non‑Egyptian number (1‑based).  The list is cached for
   subsequent calls. */
seq_t a(idx_t n)
{
    /* All non‑Egyptian numbers are <78. */
    const uint64_t LIMIT = 77;

    static bool initialized = false;
    static seq_t *non = NULL;
    static size_t count = 0;

    if (!initialized) {
        non = mem_new(seq_t, (LIMIT + 1));
        if (non == NULL) {          /* allocation failure – return 0 */
            initialized = true;
            return (seq_t)0;
        }
        count = 0;
        for (uint64_t m = 2; m <= LIMIT; ++m) {
            if (!is_egyptian(m))
                non[count++] = (seq_t)m;
        }
        initialized = true;
    }

    if ((size_t)n > count)
        return (seq_t)0;                            /* out of range */
    return non[n - 1];                              /* 1‑based index */
}
