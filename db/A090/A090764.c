/*
 * A090764  Number of partitions of n with two sorts of part 1.
 * https://oeis.org/A090764
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{partitions of n} 2^{#1's in partition} /*
 *            Recurrence: a(0) = 1 * a(n) = 2*a(n-1) + p(n) - p(n-1) for n ≥ 1
 *            * where p(n) are the partition numbers (OEIS A000041)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *part     = NULL;   /* p(n) partition numbers   */
static seq_t *part_a   = NULL;   /* a(n) sequence values     */
static size_t  used    = 0;      /* number of elements computed so far */
static size_t  cap     = 0;      /* capacity of the arrays     */

/* Ensure that the arrays contain values up to and including index n.
 * This function is thread-unsafe but the specification does not require
 * thread safety. */
static void
ensure_up_to(idx_t n)
{
    size_t require = (size_t)n + 1;   /* number of elements needed */
    if (used >= require)
        return;                      /* already enough */

    /* enlarge capacity exponentially */
    if (cap == 0) cap = 1;
    while (cap < require)
        cap <<= 1;

    part   = (seq_t *)realloc(part,   cap * sizeof(seq_t));
    part_a = (seq_t *)realloc(part_a, cap * sizeof(seq_t));

    /* compute new values */
    for (size_t i = used; i < require; ++i) {
        if (i == 0) {
            part[0]   = 1;
            part_a[0] = 1;
            continue;
        }

        /* compute partition number p(i) via pentagonal number theorem */
        seq_t pos_sum = 0;
        seq_t neg_sum = 0;
        for (int k = 1; ; ++k) {
            size_t pent1 = (size_t)k * (3 * k - 1) / 2;
            if (pent1 > i) break;
            if (k & 1)   /* odd k -> add */
                pos_sum += part[i - pent1];
            else        /* even k -> subtract */
                neg_sum += part[i - pent1];

            size_t pent2 = (size_t)k * (3 * k + 1) / 2;
            if (pent2 > i) continue;
            if (k & 1)
                pos_sum += part[i - pent2];
            else
                neg_sum += part[i - pent2];
        }
        part[i] = pos_sum - neg_sum;   /* pos_sum ≥ neg_sum */

        /* recurrence for a(i) */
        part_a[i] = 2 * part_a[i - 1] + (part[i] - part[i - 1]);
    }

    used = require;
}

/* External function required by the build. */
seq_t
a(idx_t n)
{
    ensure_up_to(n);
    return part_a[(size_t)n];
}
