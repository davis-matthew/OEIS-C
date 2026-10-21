/*
 * A087185  Partition numbers of the form 3*k+2.
 * https://oeis.org/A087185
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 45, u64 n <= 148, u128 n <= 509,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A087185: Partition numbers p(m) such that p(m) ≡ 2 (mod 3).
   The function a(n) returns the nth such partition number.
   It enumerates partition numbers with Euler's pentagonal recurrence and
   collects those whose value modulo 3 equals 2. */

/* Necessary headers */
#include <stdlib.h>
#include <stdint.h>

/* Forward declaration of helper functions for memory handling. */
static void *grow(void *ptr, size_t *cap, size_t need, size_t elem_size);

seq_t a(idx_t n)
{
    /* Return 0 for non‑positive indices (offset starts at 1). */
    if (n < (idx_t)1) {
        return 0;
    }

    /* Static storage for partition numbers p[0..] and results. */
    static seq_t *part = NULL;       /* partition numbers */
    static size_t part_cap = 0;      /* allocated capacity of part */
    static idx_t part_len = 0;       /* number of computed entries (len) */

    static seq_t *res  = NULL;       /* collected partition numbers with %3==2 */
    static size_t res_cap = 0;       /* allocated capacity of res */
    static idx_t res_len = 0;        /* number of stored terms */

    /* ---- Initialisation ---- */
    if (part == NULL) {
        part_cap = 32;
        part = (seq_t *)malloc(part_cap * sizeof(seq_t));
        if (!part) abort();
        part[0] = (seq_t)1;          /* p(0) = 1 */
        part_len = 1;                /* we have p[0] only so far */
    }

    if (res == NULL) {
        res_cap = 32;
        res = (seq_t *)malloc(res_cap * sizeof(seq_t));
        if (!res) abort();
        res_len = 0;
    }

    /* ---- Compute until we have at least n required terms ---- */
    while (res_len < n) {
        /* Compute next partition number p[part_len]. */
        idx_t cur = part_len;       /* next index to compute */
        seq_t val = 0;
        idx_t k = 1;
        int sign = 1;               /* +1 for positive, -1 for negative */

        /* Euler's recurrence over generalized pentagonal numbers. */
        while (1) {
            idx_t pent1 = k * (3 * k - 1) / 2;   /* g(k)   */
            if (pent1 > cur) break;              /* no more terms for this cur */

            seq_t term = part[cur - pent1];
            if (sign > 0) val += term; else val -= term;

            idx_t pent2 = k * (3 * k + 1) / 2;   /* h(k)   */
            if (pent2 <= cur) {
                term = part[cur - pent2];
                if (sign > 0) val += term; else val -= term;
            }

            sign = -sign;   /* flip sign for next k */
            ++k;
        }

        /* Ensure capacity for partition array and store new value. */
        if (cur >= part_cap) {
            part_cap = part_cap * 2 + 1;
            while (cur >= part_cap) part_cap <<= 1;
            part = (seq_t *)realloc(part, part_cap * sizeof(seq_t));
            if (!part) abort();
        }
        part[cur] = val;
        part_len = cur + 1;

        /* If the value meets the modulo condition, store it. */
        if (val % (seq_t)3 == (seq_t)2) {
            if (res_len >= res_cap) {
                res_cap = res_cap * 2 + 1;
                while (res_len >= res_cap) res_cap <<= 1;
                res = (seq_t *)realloc(res, res_cap * sizeof(seq_t));
                if (!res) abort();
            }
            res[res_len] = val;
            ++res_len;
        }
    }

    /* Return the requested term. */
    return res[n - 1];
}
