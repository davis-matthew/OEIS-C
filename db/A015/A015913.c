/*
 * A015913  Numbers k such that sigma(k) + 4 = sigma(k+4).
 * https://oeis.org/A015913
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 860, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 65 terms through n = 899 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for k such that sigma(k) + 4 = sigma(k + 4) */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

static seq_t *seq   = NULL;   /* array of found terms */
static size_t seq_len = 0, seq_cap = 0;
static idx_t next_k = 1;      /* next integer to test */

/* generate terms until we have at least 'need' of them */
static void ensure_sequence(idx_t need)
{
    while (seq_len < need) {
        seq_t s_k   = sigma_of(next_k);
        seq_t s_k4  = sigma_of(next_k + 4);
        if (s_k + (seq_t)4 == s_k4) {
            if (seq_len == seq_cap) {
                size_t new_cap = seq_cap ? seq_cap << 1 : 16;
                seq = seq_realloc(seq, seq_cap, new_cap);
                seq_cap = new_cap;
            }
            seq[seq_len++] = next_k;
        }
        next_k++;
    }
}

/* external function required by the test harness */
seq_t a(idx_t n)
{
    if (n == 0)                  /* undefined for offset 0 */
        return 0;
    ensure_sequence(n);
    return seq[n - 1];
}
