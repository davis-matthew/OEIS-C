/*
 * A238109  List of prefix-normal words over the alphabet {1,2}.
 * https://oeis.org/A238109
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the decimal value of the n‑th prefix‑normal word over
 *            {1,2} enumerated lexicographically (first by length, then by
 *            value)
 * Range:     u8 n <= 10, u16 n <= 32, u32 n <= 509, u64 n <= 16906,
 *            u128 n <= 16906, big any n
 * Validated: standard, pass, 57 terms through n = 9742 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

static seq_t *alloc_seq_array(size_t n)
{
    return mem_new(seq_t, n);
}

static void free_seq_array(seq_t *p)
{
    mem_free(p);
}

/* ---------- check prefix‑normal property for a prefix of length len ---------- */
static bool is_prefix_normal(const int *dig, int len)
{
    int prefOnes[64];
    prefOnes[0] = 0;
    for (int i = 0; i < len; ++i)
        prefOnes[i + 1] = prefOnes[i] + (dig[i] == 1 ? 1 : 0);

    for (int k = 1; k <= len; ++k) {
        int maxOnes = 0;
        for (int s = 0; s + k <= len; ++s) {
            int subOnes = prefOnes[s + k] - prefOnes[s];
            if (subOnes > maxOnes)
                maxOnes = subOnes;
            if (maxOnes > prefOnes[k])
                return false;                     /* early rejection */
        }
    }
    return true;
}

/* ---------- global storage for generated terms ---------- */
static seq_t *table      = NULL;   /* array of computed values   */
static size_t table_len  = 0;      /* number of stored terms     */
static size_t table_cap  = 0;      /* current array capacity     */
static int    next_len   = 1;      /* length of next word set to generate */

/* ---------- append a value to the table, growing if necessary ---------- */
static void append_term(seq_t val)
{
    if (table_len == table_cap) {
        size_t new_cap = table_cap ? table_cap * 2 : 4;
        seq_t *tmp = alloc_seq_array(new_cap);
        for (size_t i = 0; i < table_len; ++i)
            tmp[i] = table[i];
        free_seq_array(table);
        table   = tmp;
        table_cap = new_cap;
    }
    table[table_len++] = val;
}

/* ---------- depth‑first enumeration of words of a fixed length ---------- */
static void dfs_generate(int len, int pos, int *digits, seq_t cur_val)
{
    if (pos == len) {
        append_term(cur_val);
        return;
    }

    for (int d = 1; d <= 2; ++d) {
        digits[pos] = d;
        if (is_prefix_normal(digits, pos + 1)) {
            seq_t next_val = cur_val * (seq_t)10 + (seq_t)d;
            dfs_generate(len, pos + 1, digits, next_val);
        }
    }
}

/* ---------- the required sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0)               /* offset is 1; n=0 is undefined */
        return 0;

    int digits[64];
    while (table_len < (size_t)n) {
        dfs_generate(next_len, 0, digits, (seq_t)0);
        ++next_len;
    }
    return table[n - 1];
}
