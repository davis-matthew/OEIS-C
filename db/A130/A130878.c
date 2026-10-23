/*
 * A130878  Inverse Moebius transform of A100107.
 * https://oeis.org/A130878
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{d|n} d(d) * Lucas(n/d)
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where d(d) is the number of divisors of d and Lucas is the Lucas
 * sequence with Lucas(1)=1, Lucas(2)=3, Lucas(n)=Lucas(n-1)+Lucas(n-2) for n>2.
 * This equals the inverse Möbius transform of the inverse Möbius transform
 * of the Lucas numbers.
 */
#include <stdlib.h>
#include <stdint.h>

static seq_t *lucas = NULL;           /* Lucas numbers, 1‑based */
static uint64_t *d_arr = NULL;        /* divisor counts, 1‑based */
static seq_t *a_arr = NULL;           /* the desired sequence, 1‑based */
static size_t cap = 0;                /* current maximum n + 1 stored */

/* Expand all tables to support indices up to n (inclusive). */
static void ensure_capacity(idx_t n)
{
    if ((size_t)n + 1 <= cap)
        return;

    size_t new_cap = (size_t)n + 1;
    /* realloc lucas */
    seq_t *tmp_l = (seq_t *)realloc(lucas, new_cap * sizeof(seq_t));
    if (!tmp_l) abort();
    lucas = tmp_l;

    /* realloc d_arr */
    uint64_t *tmp_d = (uint64_t *)realloc(d_arr, new_cap * sizeof(uint64_t));
    if (!tmp_d) abort();
    d_arr = tmp_d;

    /* realloc a_arr */
    seq_t *tmp_a = (seq_t *)realloc(a_arr, new_cap * sizeof(seq_t));
    if (!tmp_a) abort();
    a_arr = tmp_a;

    /* initialize new entries */
    size_t i;
    /* Lucas numbers */
    if (new_cap > 1) lucas[1] = (seq_t)1;
    if (new_cap > 2) lucas[2] = (seq_t)3;
    for (i = 3; i < new_cap; ++i) {
        lucas[i] = lucas[i - 1] + lucas[i - 2];
    }

    /* divisor counts d_arr */
    for (i = 0; i < new_cap; ++i)
        d_arr[i] = 0;
    for (i = 1; i < new_cap; ++i) {
        for (size_t j = i; j < new_cap; j += i)
            ++d_arr[j];
    }

    /* compute a_arr = sum_{d|n} d(d) * Lucas(n/d) */
    for (i = 0; i < new_cap; ++i)
        a_arr[i] = (seq_t)0;
    for (i = 1; i < new_cap; ++i) {
        seq_t di = (seq_t)d_arr[i];
        seq_t li = lucas[i]; /* remainder not needed here */
        for (size_t j = i; j < new_cap; j += i) {
            a_arr[j] += di * lucas[j / i];
        }
    }

    cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* safety, not used by OEIS offset */
    ensure_capacity(n);
    return a_arr[n];
}
