/*
 * A073265  Table T(n,k) (listed antidiagonalwise in order T(1,1), T(2,1),
 *          T(1,2), T(3,1), T(2,2), ...) giving the number of compositions
 *          (ordered partitions) of n into exactly k powers of 2.
 * https://oeis.org/A073265
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 216, u16 n <= 677, u32 n <= 2232, u64 n <= 8168,
 *            u128 n <= 10011, big any n
 * Validated: standard, pass, 121 terms through n = 9031 (2026-11-03)
 * Helpers:   is_pow2, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A073265 – number of compositions of n into exactly k powers of 2
   expressed as the antidiagonal sequence a(n) = T(row,col) where
   (row,col) is obtained from n by listing T(n,k) antidiagonally.
   Recurrence: T(0,k)=T(n,0)=0, k>n -> 0,
   T(n,1)=1 if n is a power of 2 else 0,
   T(n,k)=∑_{i=0}^{⌊log₂(n‑1)⌋} T(n-2^i , k-1).   */
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/is_pow2.h"
#include "../utils/mem.h"

/* convert antidiagonal index to (row,col) -- 1‑based indexing */
static void antidiag_to_coordinates(idx_t index, idx_t *row, idx_t *col)
{
    /* find smallest s such that s*(s-1)/2 >= index */
    idx_t s = 2;
    while (((unsigned __int128)(s - 1) * s) / 2 < index)
        ++s;
    idx_t prev = (s - 1) * (s - 2) / 2;   /* terms before current diagonal */
    idx_t offset = prev + 1;
    idx_t pos = index - offset;           /* 0‑based within diagonal */
    *row = s - 1 - pos;
    *col = s - (*row);
}

/* compute the linear offset for indices (n,k) with 0 ≤ k ≤ n */
static inline idx_t idx_offset(idx_t n, idx_t k)
{
    return n * (n + 1) / 2 + k;
}

static seq_t *T_table = NULL;        /* triangular storage for T(n,k) with 0≤k≤n */
static idx_t max_allocated = 0;      /* largest n for which table is filled */

/* ensure that T(n,k) is computed for all 0 ≤ k ≤ n ≤ n_needed */
static void ensure_table(idx_t n_needed)
{
    if (n_needed <= max_allocated)
        return;

    /* new table size (triangular) */
    size_t old_size = (size_t)(max_allocated + 1) * (max_allocated + 2) / 2;
    size_t new_size = (size_t)(n_needed + 1) * (n_needed + 2) / 2;

    seq_t *new_table = mem_new(seq_t, new_size);
    if (!new_table)
        abort();                      /* out of memory */

    if (T_table) {
        mem_copy(new_table, T_table, old_size * sizeof(seq_t));
        mem_free(T_table);
    }
    mem_set(new_table + old_size, 0, (new_size - old_size) * sizeof(seq_t));
    T_table = new_table;

    /* build new rows */
    for (idx_t i = max_allocated + 1; i <= n_needed; ++i) {
        for (idx_t k = 1; k <= i; ++k) {
            seq_t val = 0;
            if (k == 1) {
                val = is_pow2((seq_t)i) ? 1 : 0;
            } else {
                /* sum over powers of two ≤ i-1 */
                seq_t power = 1;
                idx_t limit = i - 1;
                while (power <= (seq_t)limit) {
                    idx_t n2 = i - (idx_t)power;
                    idx_t kprev = k - 1;
                    if (kprev <= n2) {
                        val += T_table[idx_offset(n2, kprev)];
                    }
                    power <<= 1;
                }
            }
            T_table[idx_offset(i, k)] = val;
        }
    }
    max_allocated = n_needed;
}

/* public function: a(n) for n ≥ 1 */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    idx_t row, col;
    antidiag_to_coordinates(n, &row, &col);

    if (col > row)
        return 0;                       /* T(n,k)=0 when k>n */

    ensure_table(row);
    return T_table[idx_offset(row, col)];
}
