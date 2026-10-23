/*
 * A167703  Numbers that cannot be partitioned into distinct odd squares.
 * https://oeis.org/A167703
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 189, u16 n <= 534, u32 n <= 534, u64 n <= 534,
 *            u128 n <= 534, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A167703: numbers that cannot be partitioned into distinct odd squares, via a subset‑sum sieve. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static seq_t *table = NULL;
static idx_t table_size = 0;

static void init_sequence(void)
{
    const idx_t MAXM = 2000;          /* big enough to include all 534 terms   */
    /* boolean table of representable sums */
    bool *rep = (bool *)malloc((MAXM + 1) * sizeof(bool));
    for (idx_t i = 0; i <= MAXM; ++i)
        rep[i] = false;
    rep[0] = true;

    /* list of odd squares <= MAXM */
    idx_t squares[256];
    idx_t nsq = 0;
    for (idx_t k = 1; ; k += 2) {
        idx_t s = k * k;
        if (s > MAXM)
            break;
        squares[nsq++] = s;
    }

    /* subset‑sum DP using distinct odd squares */
    for (idx_t j = 0; j < nsq; ++j) {
        idx_t s = squares[j];
        for (idx_t sum = MAXM; sum >= s; --sum) {
            if (rep[sum - s])
                rep[sum] = true;
        }
    }

    /* collect all numbers that are NOT representable, starting from 2 */
    seq_t *tmp = (seq_t *)malloc((MAXM + 1) * sizeof(seq_t));
    idx_t cnt = 0;
    for (idx_t m = 2; m <= MAXM; ++m) {
        if (!rep[m]) {
            tmp[cnt++] = (seq_t)m;
        }
    }

    /* allocate final static table */
    table = (seq_t *)malloc((cnt + 1) * sizeof(seq_t));
    table[0] = 0;                 /* unused, keep 0 for safety */
    for (idx_t i = 0; i < cnt; ++i)
        table[i + 1] = tmp[i];

    table_size = cnt;

    free(tmp);
    free(rep);
}

seq_t a(idx_t n)
{
    /* initialize once on first call */
    static bool ready = false;
    if (!ready) {
        init_sequence();
        ready = true;
    }

    if (n == 0)                /* per OEIS offset 1, n>=1 is useful */
        return 0;
    if (n <= table_size)
        return table[n];
    /* sequence is finite; for n beyond the last term return 0 */
    return 0;
}
