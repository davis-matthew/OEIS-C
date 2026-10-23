/*
 * A135558  Sums of three distinct nonzero Fibonacci numbers.
 * https://oeis.org/A135558
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 111, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Computation: generate all sums of three distinct non‑zero Fibonacci numbers,
   sort them, remove duplicates, and return the n‑th term (1‑based). */

static int cmp_seq_t(const void *p1, const void *p2)
{
    const seq_t *a = (const seq_t *)p1;
    const seq_t *b = (const seq_t *)p2;
    if (*a < *b) return -1;
    if (*a > *b) return 1;
    return 0;
}

static bool ready = false;
static seq_t *terms = NULL;   /* sorted unique sums */
static idx_t term_count = 0;  /* number of terms stored */

seq_t a(idx_t n)
{
    if (!ready) {
        /* generate enough unique Fibonacci numbers (start with 1,2,3,5,8,…) */
        enum { MAX_FIBS = 60 };          /* more than sufficient for 10k terms */
        seq_t fib[MAX_FIBS];
        size_t fib_cnt = 0;

        fib[fib_cnt++] = 1;
        fib[fib_cnt++] = 2;              /* skip the duplicate 1 */

        while (fib_cnt < MAX_FIBS) {
            seq_t next = fib[fib_cnt - 1] + fib[fib_cnt - 2];
            /* stop if overflow would occur (unsigned wrap‑around) */
            if (next < fib[fib_cnt - 1]) break;
            fib[fib_cnt++] = next;
        }

        /* number of combinations C(fib_cnt,3) */
        size_t combos = fib_cnt * (fib_cnt - 1) * (fib_cnt - 2) / 6;
        if (combos == 0) combos = 1;      /* safety for very small fib_cnt */

        seq_t *sums = (seq_t *)malloc(combos * sizeof(seq_t));
        size_t pos = 0;

        for (size_t i = 0; i + 2 < fib_cnt; ++i) {
            for (size_t j = i + 1; j + 1 < fib_cnt; ++j) {
                seq_t partial = fib[i] + fib[j];
                for (size_t k = j + 1; k < fib_cnt; ++k) {
                    seq_t sum = partial + fib[k];
                    sums[pos++] = sum;
                }
            }
        }

        /* sort and unique the sums */
        qsort(sums, pos, sizeof(seq_t), cmp_seq_t);

        size_t uniq = 0;
        for (size_t i = 0; i < pos; ++i) {
            if (i == 0 || sums[i] != sums[i - 1]) {
                sums[uniq++] = sums[i];
            }
        }

        /* shrink to the exact size */
        seq_t *tmp = (seq_t *)realloc(sums, uniq * sizeof(seq_t));
        if (tmp) sums = tmp;

        terms = sums;
        term_count = uniq;
        ready = true;
    }

    if (n < 1 || n > term_count) {
        return 0;   /* out of range – not used by the tests */
    }

    return terms[n - 1];
}
