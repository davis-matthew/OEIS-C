/*
 * A010391  Squares mod 29.
 * https://oeis.org/A010391
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = n‑th smallest quadratic residue modulo 29 (sorted list of
 *            distinct values of k² mod 29, k = 0 … 28)
 * Range:     u8 n <= 15, u16 n <= 15, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static seq_t table[15];
static int table_len = 0;
static int table_initialized = 0;

static void init_table(void)
{
    if (table_initialized) return;
    unsigned char used[29] = {0};
    for (idx_t i = 0; i < 29; ++i) {
        seq_t r = (seq_t)i * (seq_t)i % (seq_t)29;
        used[(unsigned)r] = 1;
    }
    int pos = 0;
    for (int r = 0; r < 29; ++r) {
        if (used[r]) {
            table[pos++] = (seq_t)r;
        }
    }
    table_len = pos;
    table_initialized = 1;
}

seq_t a(idx_t n)
{
    init_table();
    if (n == 0 || n > (idx_t)table_len) return 0; /* out of range */
    return table[n - 1];
}
