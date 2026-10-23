/*
 * A162691  Strictly positive numbers k such that 24*k/(24+k) is an integer.
 * https://oeis.org/A162691
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation via divisors of 576: a(n) = 576/d - 24,
   where d runs over positive divisors of 576 less than 24.
   The resulting k‐values are returned in ascending order. */

#include <stddef.h>
#include <stdlib.h>

/* Comparator to sort seq_t values in ascending order. */
static int cmp_seq_t(const void *p1, const void *p2)
{
    const seq_t *a = (const seq_t *)p1;
    const seq_t *b = (const seq_t *)p2;
    if (*a < *b) return -1;
    if (*a > *b) return 1;
    return 0;
}

/* Lazy initialization of the sequence terms. */
static size_t _a_count = 0;
static seq_t _a_terms[24] = {0};

static void _init_terms(void)
{
    if (_a_count != 0) return;                       /* already initialized */

    seq_t temp[24];
    size_t tcnt = 0;

    for (idx_t d = 1; d < 24; ++d) {
        if ((idx_t)576 % d == 0) {                   /* d | 576                */
            seq_t k = (seq_t)((idx_t)576 / d) - 24; /* k = 576/d - 24          */
            temp[tcnt++] = k;
        }
    }
    qsort(temp, tcnt, sizeof(seq_t), cmp_seq_t);
    for (size_t i = 0; i < tcnt; ++i) {
        _a_terms[i] = temp[i];
    }
    _a_count = tcnt;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for indices < 1 */

    _init_terms();

    if ((idx_t)n > _a_count) return (seq_t)0;   /* sequence is finite */

    return _a_terms[n - 1];           /* 1‑based indexing */
}
