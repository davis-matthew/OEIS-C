/*
 * A015913  Numbers k such that sigma(k) + 4 = sigma(k+4).
 * https://oeis.org/A015913
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 860, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   seq_realloc, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for k such that sigma(k) + 4 = sigma(k + 4) */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/sigma_of.h"
#include "../utils/seq_realloc.h"

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
