/*
 * A055812  a(n) and floor(a(n)/5) are both squares; i.e., squares which
 *          remain squares when written in base 5 and last digit is removed.
 * https://oeis.org/A055812
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 16, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   This function generates the OEIS sequence A055812.
   The sequence comprises square numbers k² such that floor(k²/5) is also a
   perfect square.  The set of k values is produced by merging the four
   Pell-type recurrences
          x' = 9x + 20y ,   y' = 4x + 9y
   having the initial seeds
          (1,0), (2,0), (3,1) and (7,3).
   Each recurrence yields an increasing sequence of k values; their squares
   are merged in sorted order to form a(n).
*/

#include <stdlib.h>

static seq_t *seq_values = NULL;
static size_t seq_len   = 0;
static size_t seq_cap   = 0;

static struct PellSeq {
    seq_t x;
    seq_t y;
} pell[4];

static int pell_init = 0;

static void ensure_capacity(size_t needed)
{
    if (needed <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 4;
    while (needed > new_cap) new_cap <<= 1;
    seq_values = (seq_t *)realloc(seq_values, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

static void pell_next(struct PellSeq *p)
{
    seq_t nx = (seq_t)9 * p->x + (seq_t)20 * p->y;
    seq_t ny = (seq_t)4 * p->x + (seq_t)9 * p->y;
    p->x = nx;
    p->y = ny;
}

static void init(void)
{
    if (pell_init) return;
    pell_init = 1;

    ensure_capacity(1);
    seq_values[0] = (seq_t)0;
    seq_len = 1;

    pell[0].x = (seq_t)1; pell[0].y = (seq_t)0;
    pell[1].x = (seq_t)2; pell[1].y = (seq_t)0;
    pell[2].x = (seq_t)3; pell[2].y = (seq_t)1;
    pell[3].x = (seq_t)7; pell[3].y = (seq_t)3;
}

seq_t a(idx_t n)
{
    init();
    while (seq_len < (size_t)n) {
        seq_t cand[4];
        for (int i = 0; i < 4; ++i) cand[i] = pell[i].x * pell[i].x;
        seq_t next = cand[0];
        int idx_min = 0;
        for (int i = 1; i < 4; ++i) {
            if (cand[i] < next) { next = cand[i]; idx_min = i; }
        }
        ensure_capacity(seq_len + 1);
        seq_values[seq_len++] = next;
        for (int i = 0; i < 4; ++i)
            if (cand[i] == next) pell_next(&pell[i]);
    }
    return seq_values[(size_t)n - 1];
}
